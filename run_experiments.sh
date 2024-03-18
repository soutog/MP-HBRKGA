#!/bin/bash

#echo "-----------------------------sem Busca Local - Sem Local Branching-------------------------------------------------------">>experimentos100.txt 
#for instancia in $(find data/ -name "*100.txt" | sort -t '/' -k2.5)
#do
#	bin/Tscfl100 $instancia 100 10 80 0.90 >> experimentos100.txt 
#done

#bin/Tscfl100_BL data/PSC4-C5-100.txt 100 10 80 0.90 >> experimentos100.txt 
#bin/Tscfl100_BL data/PSC5-C5-100.txt 100 10 80 0.90 >> experimentos100.txt 

#echo "-----------------------------com Busca Local - Sem Local Branching-------------------------------------------------------">>experimentos100.txt 
#for instancia in $(find data/ -name "*100.txt" | sort -t '/' -k2.5)
#do
#	bin/Tscfl100_BL $instancia 100 10 80 0.90 >> experimentos100.txt 
#done

echo "-----------------------------Instancia Multi-stage BRKGA 50 5-------------------------------------------------------"  >>experimentos50-5_final.txt
for instancia in $(find data/ -name "*50-5.txt" | sort -t '/' -k2.5)
do

	bin/mptscfl $instancia 100 5 100 0.75 >> experimentos50-5_final.txt 
done

# echo "-----------------------------Instancia Multi-stage BRKGA 50 10 -------------------------------------------------------"  >> experimentos50-10_final.txt
# for instancia in $(find data/ -name "*50-10.txt" | sort -t '/' -k2.5)
# do
# 	bin/mptscfl $instancia 100 5 100 0.75  >> experimentos50-10_final.txt
# done

# echo "-----------------------------Instancia Multi-stage BRKGA 100 5-------------------------------------------------------"  >>experimentos100-5_final.txt
# for instancia in $(find data/ -name "*100-5.txt" | sort -t '/' -k2.5)
# do
# 	bin/mptscfl $instancia 100 5 100 0.75 >> experimentos100-5_final.txt 
# done

# echo "-----------------------------Instancia Multi-stage BRKGA 100 10-------------------------------------------------------"  >>experimentos100-10_final.txt
# for instancia in $(find data/ -name "*100-10.txt" | sort -t '/' -k2.5)
# do
# 	bin/mptscfl $instancia 100 5 100 0.75 >> experimentos100-10_final.txt 
# done

