#!/bin/bash



file=./log.txt

#-n1 leggi parola per parola
#-r legge riga per riga


while read line; do 
  echo $line 
  for parola in $line
    do
        echo "[-]!" $parola

            lung=${#parola}

    done
done < "$file"