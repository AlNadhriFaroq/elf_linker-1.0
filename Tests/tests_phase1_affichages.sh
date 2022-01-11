#!/bin/bash


# Test l'affichage de l'en-tete
function test_header()
{	../Phase1/readelf -h $1 > resultat_projet_temp.txt
	arm-none-eabi-readelf -h $1 > resultat_fourni_temp.txt
	
	# Mise en forme pour retirer la ligne indiquant la machine
	grep -v "Machine:" resultat_projet_temp.txt > resultat_projet.txt
	grep -v "Machine:" resultat_fourni_temp.txt > resultat_fourni.txt

	diff resultat_projet.txt resultat_fourni.txt > /dev/null
	if [[ $? -eq 0 ]]
	then
		echo "TEST AFFICHAGE HEADER : IDENTIQUE"
	else
		echo "TEST AFFICHAGE HEADER : DIFFERENT"
		diff --suppress-common-lines resultat_projet.txt resultat_fourni.txt
	fi
	echo ""
}


# Test l'affichage de la table des sections
function test_sect_table()
{	../Phase1/readelf -S $1 > resultat_projet.txt
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
}


# Test l'affichage du contenu d'une section
function test_section()
{	../Phase1/readelf -x $2 $1 > resultat_projet.txt
	arm-none-eabi-readelf -x $2 $1 | cut -d " " -f 1-7 > resultat_fourni_temp.txt

	# Mise en forme du cas ou une note est presente dans le resultat fourni et suppression de la note
	grep -v " NOTE:" resultat_fourni_temp.txt > resultat_fourni.txt

	# Test l'affichage d'une section
	diff -w -b -B resultat_projet.txt resultat_fourni.txt > /dev/null
	if [[ $? -eq 0 ]]
	then
		echo "    Test affichage section $num : Identique"
	else
		echo "    Test affichage section $num : Different"
		diff -w -b -B --suppress-common-lines resultat_projet.txt resultat_fourni.txt
		echo ""
	fi
}


function test_sections()
{	# Recuperation du nombre de section reel
	let nb_sec=$(arm-none-eabi-readelf -S $1 | head -n 1 | cut -d " " -f 3)-1
	
	echo "TEST AFFICHAGE SECTIONS :"
	
	# Parcours des sections
	for num in $(seq 0 $nb_sec)
	do
		test_section $1 $num
	done
	echo ""
}


# Test l'affiche de la table des symboles
function test_symb_table()
{	../Phase1/readelf -s $1 > resultat_projet.txt
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
}


# Test l'affichage de la table de reimplentation
function test_reimp_table()
{	../Phase1/readelf -r $1 > resultat_projet.txt
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
}


# Tests les affichages des fonctions de la phase 1
function tests()
{	if [[ -e $1 ]]
	then
		test_header $1
		test_sect_table $1
		test_sections $1
		test_symb_table $1
		test_reimp_table $1
		rm resultat_projet.txt resultat_fourni.txt resultat_projet_temp.txt resultat_fourni_temp.txt
	else
		echo "Error: File '$1' not find"
	fi
}


tests $1
