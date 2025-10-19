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
      directorios+=($PWD);
      echo ${directorios}
fi


echo "Nivel de precisión: ${precision}"
echo "Profundidad: ${profundidad}"
echo "Tamaño minimo en bytes: ${bytes}"
echo "Rutas a procesar: ${directorios[@]}"
echo ""

archivos=()

## Función recursiva que analiza los archivos del directorio
## ------------------------------------------------------------------------------------------- ##
## ------------------------------------------------------------------------------------------- ##
analizarArchivos() { #* Parámetros :: profundidadActual, directorio
    local profundidadActual=$1  # Local para que no choque con las demás variables
    local directorio=$2

    # Verificar si el directorio existe
    if [ ! -d "$directorio" ]; then
        echo "El directorio no existe : $directorio" >&2
        return
    fi

    local archivos=("${directorio}"/*)

    for archivo in "${archivos[@]}"; do
        if [[ -d $archivo && $profundidadActual -ne 0 ]]; then
            #Si es un archivo y no llegamos al tope de profundidad, analizar los archivos de adentro con profundidad - 1.
            analizarArchivos $((profundidadActual - 1)) "$archivo"
        elif [[ -f $archivo ]]; then
            # Si es un archivo, mando la información por echo para ser capturado por mapfile
            # Usando realpath para luego poder conseguir la información necesaria por otros medios.
            echo "$(realpath "$archivo")"
        fi
    done
}

## ------------------------------------------------------------------------------------------- ##
## ------------------------------------------------------------------------------------------- ##
## ------------------------------------------------------------------------------------------- ##

## Función para recorrer todos los directorios que me dieron. Existe para facilitarme el mapfile.
analizarDirectorios() {
    for directorio in "${directorios[@]}"; do
        analizarArchivos "$profundidad" "$directorio"
    done
}

## ------------------------------------------------------------------------------------------- ##
## ------------------------------------------------------------------------------------------- ##

## ORDENO LOS ARCHIVOS Y LOS ALMACENO EN UN NUEVO ARRAY
## ------------------------------------------------------------------------------------------- ##
mapfile -t archivosOrdenados < <(
    analizarDirectorios \
    | while read -r ruta; do
        nombre=$(basename "$ruta")
        echo -e "${nombre}\t${ruta}"
      done \
    | sort -V -t$'\t' -k1,1 \
    | cut -f2-
)
## mapfile --> lee la salida en formato de array.
## El while lee las rutas y (-r para que no haya errores por culpa de tabuladores) y crea un nuevo echo con el nombre de primer valor, así luego ordeno por eso
## Luego, el sort va a ordenar por el orden natural '-V' diciéndole que el separador de campos (-t) es el tabulador ($'\t'). Y lo que va a hacer es ordenar por el primer campo, el nombre
##            (-k1,1   k es que campos o columnas va a usar para el orden, y el parametro es 1,1; es decir, desde la columna 1 hasta la 1.)
## cut -f2- --> Está cortando a partir del segundo field, es decir, borra completamente el nombre

contador=0 ## Para manejar la posición del array

while (( contador < ${#archivosOrdenados[@]} - 1 )); do
    nombreArchivoActual="$(basename "${archivosOrdenados[contador]}")" 
    nombreArchivoSiguiente="$(basename "${archivosOrdenados[$((contador + 1))]}")"

    # Si la precisión es 1 solo comparará los nombres.
    if [[ $precision -eq 1 ]]; then
        if [[ "$nombreArchivoActual" == "$nombreArchivoSiguiente" ]]; then
            echo "Se encontraron archivos duplicados!"
            echo -e "$(basename "${archivosOrdenados[contador]}")\tbytes: $(stat -c%s "${archivosOrdenados[contador]}")\t$(realpath "${archivosOrdenados[contador]}")"
            echo -e "$(basename "${archivosOrdenados[$((contador + 1))]}")\tbytes: $(stat -c%s "${archivosOrdenados[$((contador + 1))]}")\t$(realpath "${archivosOrdenados[$((contador + 1))]}")"
            echo ""
        fi
    fi

    # Si la precisión es 2, compara nombres y tamaño
    if [[ $precision -eq 2 ]]; then
        tamañoActual=$(stat -c%s "${archivosOrdenados[contador]}")
        tamañoSiguiente=$(stat -c%s "${archivosOrdenados[$((contador + 1))]}")

        if [[ "$nombreArchivoActual" == "$nombreArchivoSiguiente" && $tamañoActual -eq $tamañoSiguiente ]]; then
            echo "Se encontraron archivos duplicados tanto por nombre como por tamaño!"
            echo -e "$(basename "${archivosOrdenados[contador]}")\tbytes: $(stat -c%s "${archivosOrdenados[contador]}")\t$(realpath "${archivosOrdenados[contador]}")"
            echo -e "$(basename "${archivosOrdenados[$((contador + 1))]}")\tbytes: $(stat -c%s "${archivosOrdenados[$((contador + 1))]}")\t$(realpath "${archivosOrdenados[$((contador + 1))]}")"
            echo ""
        fi
    fi

    (( contador++ ))
done