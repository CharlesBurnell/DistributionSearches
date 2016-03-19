# DistributionSearches

This is a c program that takes user input and checks
how fast different search algorithms find the desired
point.

Arguments are -d -s topFloor bottomFloor numberOfRounds <mu|lambda> <sigma>
where -d is -u for Unform -e for Exponential -n for Normal
and where -s is -b for binary search -l for linear search
no optional arguments passed for uniform
for Exponential only lambda
for Normal both mu and sigma

Makefile has the following.
make just compiles
make examples runs some basic test cases
make clean removes compiled objects.
