#!/bin/sh
set -eu
output=$(${CCOWSAY_TEST_COWSAY:-./ccowsay} Hello ALT)
printf '%s\n' "$output" | grep '< Hello ALT >'
printf '%s\n' "$output" | grep '\^__\^'
