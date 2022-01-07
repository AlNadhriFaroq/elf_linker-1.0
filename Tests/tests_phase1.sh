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
	./readelf -x $num $1 > resultat_projet.txt
	arm-none-eabi-readelf -x $num $1 | cut -d " " -f 1-7 > resultat_fourni.txt

	# Mise en forme du cas ou la section est vide conformement a notre resultat
	no_dump=$(cut -d " " -f 3-7 resultat_fourni.txt)
	if [[ "$no_dump" == "has no data to dump." ]]
	then
		# Si la section est bien vide, alors on modifie
		nom_section=$(cut -d " " -f 2 resultat_fourni.txt)
		echo "" > resultat_fourni.txt
		echo "Hex dump of section $nom_section:" >> resultat_fourni.txt
		echo "  0x00000000" >> resultat_fourni.txt
	fi

	# Mise en forme du cas ou une note est presente dans le resultat fourni et suppression de la note
	grep -v " NOTE:" resultat_fourni.txt > resultat_fourni_grep.txt

	# Test l'affichage d'une section
	diff -w -b -B resultat_projet.txt resultat_fourni_grep.txt > /dev/null
	if [[ $? -eq 0 ]]
	then
		echo "    Test affichage section $num : Identique"
	else
		echo "    Test affichage section $num : Different"
		diff -w -b -B --suppress-common-lines resultat_projet.txt resultat_fourni_grep.txt
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

rm resultat_projet.txt resultat_fourni.txt resultat_fourni_grep.txt
make clean > /dev/null
cd ../Tests
