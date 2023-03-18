cd ../../Settings
declare -a ARRAY_OF_SettingsFILES=("Settings_257_to_263_09_09_2020_BG_Subs" "Settings_334_to_339_09_09_2020_BG_Subs" "Settings_390_to_393_09_09_2020_BG_Subs" "Settings_399_to_403_09_09_2020_BG_Subs" "Settings_411_to_416_09_09_2020_BG_Subs" "Settings_438_to_456_09_09_2020_BG_Subs" "Settings_438_to_456_09_09_2020" "Settings_483_to_488_09_09_2020_BG_Subs" "Settings_658_to_665_09_09_2020_BG_Subs" "Settings_940_to_949_09_09_2020_BG_Subs")
fileType=".txt"
toExcel="ToExcel"
for s in "${ARRAY_OF_SettingsFILES[@]}"
do


	textFile=$s$fileType
	echo $textFile
	fileName=$(grep "Output_File_Name: " GammaElectron_S12/NewGates/$textFile|cut -c19-)
	echo $fileName
	cp GammaElectron_S12/NewGates/$textFile Settings.txt
	read -p "Press enter to continue"
	./../Codes/GaussianFitter
	toExcelFile=$fileName$toExcel$fileType
done
#./../Codes/GaussianFitter
#cp SumElectron/Settings_Final_540to620_01_06_2020_WithoutS12A.txt Settings.txt
#read -p "Press enter to continue"
#./../Codes/GaussianFitter
