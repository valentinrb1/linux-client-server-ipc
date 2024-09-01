#!/bin/bash

for (( i=0; i<$1; i++ )); do
    ../bin/clients A ../files/server_id.txt &
    sleep 0.15
    ../bin/clients B ../files/server_id.txt &
    sleep 0.15
    ../bin/clients C ../files/server_id.txt &
    sleep 0.15

    echo "$i"
done