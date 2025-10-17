# Resource Management

Using libcgroup to maintain process cpu/cpuset/memory

## Usage

- start service: `sudo resourcectl start`
- restart service: `sudo resourcectl restart`
- stop service: `sudo resourcectl stop`
- show service log: `sudo resourcectl log`
- check group debug checking: `sudo resourcectl debug`

## Requirement

`apt install cgroup-tools`
