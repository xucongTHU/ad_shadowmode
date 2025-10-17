import ftplib
import argparse
import os
import logging
from tqdm import tqdm

logger = logging.getLogger("FTP_UPLOAD")


def list_recursive(ftp, remotedir):
    out_files = []
    out_directory = []
    try:
        ftp.cwd(remotedir)
    except ftplib.error_perm as e:
        if "No such file or directory" in str(e):
            return [], []

    out_directory.append(remotedir)

    if len(remotedir) > 0 and remotedir[-1] != '/':
        remotedir += '/'
    for entry in ftp.mlsd():
        if entry[0] == "." or entry[0] == "..":
            continue
        if entry[1]['type'] == 'dir':
            remotepath = remotedir + entry[0]
            tout_files, tout_directory = list_recursive(ftp, remotepath)
            out_files.extend(tout_files)
            out_directory.extend(tout_directory)
        else:
            out_files.append(remotedir + entry[0])
    return out_files, out_directory


def upload_file(ftp, filepath, dest_ftp_path):
    with open(filepath, 'rb') as fp:
        ftp.storbinary('STOR %s' % (dest_ftp_path), fp)


def upload_directory_to_ftp_server(server, username, password, local_src, ftp_dest):
    if not os.path.exists(local_src):
        logger.error("Local directory not exist: %s", local_src)
        exit(1)
    logger.info("- Try Login to [%s:%s@%s]", username, password, server)
    session = ftplib.FTP(server, username, password)
    logger.info("- Login Success")

    ftp_dest_dir = os.path.join(ftp_dest, os.path.basename(local_src))
    logger.info("- Retreive old file list in server [%s]", ftp_dest_dir)
    old_files, old_directory = list_recursive(session, ftp_dest_dir)
    logger.info("- Retreive %d file, %d directory",
                len(old_files), len(old_directory))
    old_directory.sort(
        reverse=True, key=lambda s: len(s.strip("/").split("/")))

    logger.info("- Start Delete")
    for f in tqdm(old_files):
        session.delete(f)
    for d in old_directory:
        session.rmd(d)
    logger.info("- Done Delete")

    logger.info("- Start Upload")
    upload_list = []
    session.mkd(ftp_dest_dir)
    for root, dirs, files in os.walk(local_src):
        for f in files:
            src_f = os.path.join(root, f)
            dst_p = os.path.join(ftp_dest, os.path.join(root, f))
            upload_list.append((src_f, dst_p))
        for d in dirs:
            dst_p = os.path.join(ftp_dest, os.path.join(root, d))
            session.mkd(dst_p)
    for src_f, dst_p in tqdm(upload_list):
        upload_file(session, src_f, dst_p)
    logger.info("- Done Upload")

    # session.delete


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", help="ftp server ip")
    parser.add_argument("-u", "--user", help="ftp server username")
    parser.add_argument("-p", "--password", help="ftp server password")
    parser.add_argument("-i", "--local_src", help="source directory to upload")
    parser.add_argument("-d", "--ftp_dest",
                        help="destination directory path in ftp")

    args = parser.parse_args()
    upload_directory_to_ftp_server(
        args.ip, args.user, args.password, args.local_src, args.ftp_dest)

    exit(0)
