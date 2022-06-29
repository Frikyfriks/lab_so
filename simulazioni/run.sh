#!/bin/bash
if [[ -f $1 ]]
 then
    rm $1
fi    
./program $1 $2
