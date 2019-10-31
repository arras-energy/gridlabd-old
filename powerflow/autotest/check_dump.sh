#!/bin/bash
if [ ! -f solver_branch.csv -o ! -f solver_bus.csv ]; then
    exit 1
else
    exit 0
fi