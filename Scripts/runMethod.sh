#!/bin/sh

x=`basename $1 | sed -e "s/.gra//g"`
echo $x
for i in 1 2 4 6 8
 do
  echo $i "queries per update"
  echo "Running DFS"
  ../DGRAIL/DGRAIL -g $1 -t $2 -type dfs -qpu $i > depo/${x}_dfs_$i -n 1000
  echo "Running Dagger 0"
  ../DGRAIL/DGRAIL -g $1 -t $2 -type mg -dim 0 -qpu $i > depo/${x}_mg0_$i -n 1000
  echo "Running Dagger 1"
  ../DGRAIL/DGRAIL -g $1 -t $2 -type mg -dim 1 -qpu $i > depo/${x}_mg1_$i -n 1000
  echo "Running Dagger 2"
  ../DGRAIL/DGRAIL -g $1 -t $2 -type mg -dim 2 -qpu $i > depo/${x}_mg2_$i -n 1000
done

