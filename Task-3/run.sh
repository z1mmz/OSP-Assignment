#!/usr/bin/env bash
./external442t 10 1 3495671 &
./external442t 100 2 3495671 &
./external442t 1000 3 3495671 &
./external442t 10000 4 3495671 &
./external442t 50 5 3495671200 &
./external442t 500 6 3495671200 &
./external442t 5000 7 3495671200 &
./external442t 50000 8 3495671200 &
./central442t 10 3495671 3495671200
