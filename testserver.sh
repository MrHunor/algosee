#!/bin/bash
 #   algosee; A algorithm visulizer
 #   Copyright (C) 2026  MrHunor, siryanni (as equals)
 #
 #   This program is free software: you can redistribute it and/or modify
 #   it under the terms of the GNU General Public License as published by
 #   the Free Software Foundation, either version 3 of the License, or
 #   (at your option) any later version.
 #
 #   This program is distributed in the hope that it will be useful,
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #   GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License
 #   along with this program.(root/LICENSE)  If not, see <https://www.gnu.org/licenses/>.
#!/bin/bash
#Disclaimer: ai helped a lot with this 
./build/algosee &
echo "Started server in the background"

algos=("selection" "bubble" "quick" "merge" "counting" "cycle")

for algo in "${algos[@]}"; do
    for i in 20 100 500 1000 2000 2499; do



   response=$(curl -s -X POST \
    "http://127.0.0.1:8080/sortalgo?algo=$algo" \
    -H "Content-Type: application/json" \
    -d "{\"values\":[$(seq 1 "$i" | shuf | paste -sd, -)]}")



         sortedReply=$(echo "$response" | jq -c '.SORTED')

expected=$(seq 1 "$i" | jq -R -s -c 'split("\n") | map(select(length > 0) | tonumber)')

        
if [ "$expected" != "$sortedReply" ]; then
    echo "FAILED: $algo, $i"
     echo "Expected: $expected"
         echo "Received: $sortedReply"

    pkill algosee
    exit 1
fi

        echo "Passed: $algo with $i elements"
    done
done

pkill algosee
exit 0