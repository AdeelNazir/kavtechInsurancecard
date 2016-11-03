<?php


switch($_POST["functionname"])
{ 

case 'getMRZFields':
	ini_set('max_execution_time', 300); 

	echo "start ececution \n";
	exec("./PassportOCR",$output);
	echo "\n executed";
	foreach($output as $line)
		echo $line;


			break;    

case 'methodCall':
//get face data code
		break; 	

case 'checkimage':
//get thumb data code
		break; 	
		
}   




	?>

