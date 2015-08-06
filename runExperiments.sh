for i in *.txt; 
do  echo "$i"; ./CharikarPeel.exe  < "$i" > "$i-Charikar-results"; ./TrianglePeel.exe  < "$i" > "$i-Triangle-results"; done
