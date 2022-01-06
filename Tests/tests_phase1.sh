#!/bin/bash

cd ../Phase1
make > /dev/null

# Test l'affiche du header
./readelf -h $1 > resultat_projet.txt
arm-none-eabi-readelf -h $1 > resultat_fourni.txt
diff resultat_projet.txt resultat_fourni.txt > /dev/null
if [[ $? -eq 0 ]]
then
	echo "TEST AFFICHAGE HEADER : IDENTIQUE"
else
	echo "TEST AFFICHAGE HEADER : DIFFERENT"
	diff --suppress-common-lines resultat_projet.txt resultat_fourni.txt
fi
echo ""

# Test l'affichage de la table des sections
./readelf -S $1 > resultat_projet.txt
arm-none-eabi-readelf -S $1 > resultat_fourni.txt
diff resultat_projet.txt resultat_fourni.txt > /dev/null
if [[ $? -eq 0 ]]
then
	echo "TEST AFFICHAGE TABLE DES SECTIONS : IDENTIQUE"
else
	echo "TEST AFFICHAGE TABLE DES SECTIONS : DIFFERENT"
	diff --suppress-common-lines resultat_projet.txt resultat_fourni.txt
fi
echo ""

# Recuperation du nombre de section reel
let nb_sec=$(head -n 1 resultat_fourni.txt | cut -d " " -f 3)-1

# Parcours des sections
echo "TEST AFFICHAGE SECTIONS :"
for num in $(seq 0 $nb_sec)
do
	# Test l'affichage d'une section
	./readelf -x $num $1 > resultat_projet.txt
	arm-none-eabi-readelf -x $num $1 | cut -d " " -f 1-7 > resultat_fourni.txt
	diff -w -b -B resultat_projet.txt resultat_fourni.txt > /dev/null
	if [[ $? -eq 0 ]]
	then
		echo "    Test affichage section $num : Identique"
	else
		echo "    Test affichage section $num : Different"
		diff -w -b -B --suppress-common-lines resultat_projet.txt resultat_fourni.txt
		echo ""
	fi
done
echo ""

# Test l'affiche de la table des symboles
./readelf -s $1 > resultat_projet.txt
arm-none-eabi-readelf -s $1 > resultat_fourni.txt
diff resultat_projet.txt resultat_fourni.txt > /dev/null
if [[ $? -eq 0 ]]
then
	echo "TEST AFFICHAGE TABLE DES SYMBOLES : IDENTIQUE"
else
	echo "TEST AFFICHAGE TABLE DES SYMBOLES : DIFFERENT"
	diff --suppress-common-lines resultat_projet.txt resultat_fourni.txt
fi
echo ""

# Test l'affichage de la table de reimplentation
./readelf -r $1 > resultat_projet.txt
arm-none-eabi-readelf -r $1 > resultat_fourni.txt
diff resultat_projet.txt resultat_fourni.txt > /dev/null
if [[ $? -eq 0 ]]
then
	echo "TEST AFFICHAGE TABLE DE REIMPLANTATION : IDENTIQUE"
else
	echo "TEST AFFICHAGE TABLE DE REIMPLANTATION : DIFFERENT"
	diff --suppress-common-lines resultat_projet.txt resultat_fourni.txt
fi
echo ""

rm resultat_projet.txt resultat_fourni.txt
make clean > /dev/null
cd ../Tests
