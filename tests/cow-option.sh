#!/bin/sh
set -eu
output=$(${CCOWSAY_TEST_COWSAY:-./ccowsay} --cow sheep baa)
printf '%s\n' "$output" | grep '< baa >'
printf '%s\n' "$output" | grep '|||  |||'
