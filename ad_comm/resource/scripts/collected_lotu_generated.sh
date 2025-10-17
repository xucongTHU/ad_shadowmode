#! /bin/bash


SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Step 1. Check whether arxml is installed
if ! command -v arxmlc &> /dev/null
then
    echo "arxmlc could not be found"
    exit
fi

# Step 1. Delete the existing
rm -rf $SCRIPT_DIR/../generated/lotu_generated

# Step 2. Generate
arxmlc -p rscl_lotu -i  ${SCRIPT_DIR}/../generated/lotu_arxml/ -o $SCRIPT_DIR/../generated/lotu_generated/

# Step 3. Done
echo "Done"

