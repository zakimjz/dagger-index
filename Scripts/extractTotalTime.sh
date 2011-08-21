#!/bin/sh

x=`basename $1`
echo $x
rm tmp
rm ${x}.tt
echo "#\tDFS\tDG0\tDG1\tDG2" > ${x}.tt
for i in 1 2 4 6 8 10 12 14 16 18 20
 do
  echo $i > tmp
  grep "Total Time"  depo/${x}_dfs_$i | sed 's\.*Total Time = \\g' >> tmp
  grep "Total Time"  depo/${x}_mg0_$i | sed 's\.*Total Time = \\g' >> tmp
  grep "Total Time"  depo/${x}_mg1_$i | sed 's\.*Total Time = \\g' >> tmp
  grep "Total Time"  depo/${x}_mg2_$i | sed 's\.*Total Time = \\g' >> tmp
  tr '\n' '\t' < tmp >> ${x}.tt
  echo  "" >> ${x}.tt
done
rm tmp
