#!/bin/bash


function test_ecriture()
{	../Phase1/writeelf $1 $2
	
	diff $1 $2 > /dev/null
	if [[ $? -eq 0 ]]
	then
		echo "TEST ECRITURE : IDENTIQUE"
	else
		echo "TEST ECRITURE : DIFFERENT"
		diff $1 $2
	fi
}

# Tests les ecritures des fonctions de la phase 1
function tests()
{	if [[ -e $1 ]]
	then
		test_ecriture $1 $2
		rm $2
	else
		echo "Error: File '$1' not find"
	fi
}


tests $1 $2
