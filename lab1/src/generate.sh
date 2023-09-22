#!/bin/bash
for i in `seq 150`; do od -vAn -N2 -tu2 < /dev/random; done > $@
