#!/bin/bash

ok=1
alias longest="NA"
length=-1
while [ $ok -eq 1 ]
do
  read -p "Inserisci parole " parola
  if [ $parola != "quit" ]
  then
    lung=${#parola}
    if [ $length -lt $lung ]
    then
      longest=$parola
      length=$lung
    fi
  else
    ok=0
  fi
done
>&1 echo $longest #stdout
>&2 echo $length #stderr
echo $longest > alias.txt
