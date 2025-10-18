# *midup [-p PROFUNDIDAD] [-l NIVEL DE PRECISIÓN] [-s BYTES] [DIRECTORIO...]

args=("$@")
profundidad=""
precision=""
bytes=""
directorios=()

contador=0  # Para manejar los parámetros, uso un contador

# * Guardo cada parámetro en su respectiva variable
while (( contador < ${#args[@]} )); do  # Mientras el contador no haya superado el número de parámetros
    case ${args[contador]} in
        -p)
            if [[ -z ${args[$((contador + 1))]} || ${args[$((contador + 1))]} == -* ]]; then
                echo "Error: -p requiere un argumento "
                exit 1
            fi  # Este if valida que el parámetro tenga algo después y no el siguiente parámetro
            profundidad=${args[$((contador + 1))]}
            (( contador += 2 ))
            ;;
        -s)
            if [[ -z ${args[$((contador + 1))]} || ${args[$((contador + 1))]} == -* ]]; then
                echo "Error: -s requiere un argumento "
                exit 1
            fi  # Este if valida que el parámetro tenga algo después y no el siguiente parámetro
            bytes=${args[$((contador + 1))]}
            (( contador += 2 ))
            ;;
        -l)
            if [[ -z ${args[$((contador + 1))]} || ${args[$((contador + 1))]} == -* ]]; then
                echo "Error: -l requiere un argumento "
                exit 1
            fi  # Este if valida que el parámetro tenga algo después y no el siguiente parámetro
            precision=${args[$((contador + 1))]}
            (( contador += 2 ))
            ;;
        *)
            directorios+=(${args[$contador]})
            (( contador++ ))
            ;;
    esac
done


# * Validación de errores * #
# ************************* #
if [ -z $profundidad ]; then ##Si no se ha puesto profundidad, se pone a 0
      profundidad=0;
fi
if ! [[ $profundidad =~ ^[0-9]+$ ]]; then #Compruebo si la profundidad es un número.
   echo "La profundidad debe ser un número."
   echo "Tu valor : ${profundidad}"
   exit ## Corto la ejecución del programa
fi


if ! [[ $precision =~ ^[1-2]$ ]]; then #Compruebo si la precision es 1 o 2. Como tiene que ser un dígito quité el '+'
   echo "La precisión debe ser o bien 1 o bien 2"
   echo "Tu valor : ${precision}"
   exit ## Corto la ejecución del programa
fi


if ! [[ $bytes =~ ^[1-9]+$ ]]; then # Compruebo si el tamaño es un número
   echo "El tamaño tiene que ser un número."
   echo "Tu valor : ${bytes}"
   exit ## Corto la ejecución del programa
fi


if [ -z $directorios ]; then ##Si no se ha puesto profundidad, se pone a 0
      directorios+=("./");
      echo ${directorios}
fi


echo "Nivel de precisión: ${precision}"
echo "Profundidad: ${profundidad}"
echo "Tamaño minimo en bytes: ${bytes}"
echo "Rutas a procesar: ${directorios[@]}"
echo ""


#* Ver que archivos hay
for directorio in "${directorios[@]}"; 
do

    if [ ! -d "$directorio" ]; then
        echo "Este directorio está mal, no existe : ${directorio}"
        echo "Terminando el programa"
        exit
    fi

    archivos=("${directorio}"/*)

    for archivo in "${archivos[@]}"; 
    do
        echo "$(basename "$archivo")  bytes: $(stat -c%s "$archivo")   $(realpath "$archivo")"
    done
done | sort



echo "${archivos[@]}"