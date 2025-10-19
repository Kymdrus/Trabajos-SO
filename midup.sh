#!/bin/bash
argumentol=0
argumentop=-1
argumentos=0
archivosnombre=()
archivosruta=()
aux=()
espera_argumento=""
archivostamano=()
carpetaBoolean=false
buscar(){
local Carpeta=$1
local contador=$2
if [ "$argumentop" -ne -1 ] && [ $contador -gt $argumentop ]; then 
     return 
fi  
if [ -d "$Carpeta" ]; then
   
    for archivo in "$Carpeta"/*; do
        if [ -d "$archivo" ]; then
            buscar "$archivo" $((contador+1))
        fi
    done
    for archivo in "$Carpeta"/*; do
        if [ -f "$archivo" ]; then
           nombre=$(basename "$archivo")
           tamano=$(stat -c%s "$archivo")
           ruta=$(realpath "$archivo")
           archivosnombre+=("$nombre")
           archivosruta+=("$ruta")
           archivostamano+=("$tamano")
                                                                                    

          fi
     done
                           
fi 
 }
ordenar_arrays_por_nombre() {
    local -n nombres_ref=$1
    local -n rutas_ref=$2
    local -n tamanos_ref=$3

    # Unir los tres arrays en una representación de texto
    local combinado=""
    for i in "${!nombres_ref[@]}"; do
        combinado+="${nombres_ref[$i]}|${rutas_ref[$i]}|${tamanos_ref[$i]}"$'\n'
    done

    # Ordenar alfabéticamente por el primer campo (nombre)
    local combinado_ordenado
    combinado_ordenado=$(sort <<< "$combinado")

    # Vaciar los arrays originales
    nombres_ref=()
    rutas_ref=()
    tamanos_ref=()

    # Reconstruir los arrays a partir del texto ordenado
    while IFS='|' read -r nombre ruta tamano; do
        [[ -z "$nombre" ]] && continue  # saltar líneas vacías
        nombres_ref+=("$nombre")
        rutas_ref+=("$ruta")
        tamanos_ref+=("$tamano")
    done <<< "$combinado_ordenado"
}

 Comparacion() {
    local Anterior=""
    local RutaAnterior=""
    tamanoanterior=0

    # Ordenar los arrays antes de comparar
    ordenar_arrays_por_nombre archivosnombre archivosruta archivostamano

    for i in "${!archivosnombre[@]}"; do
        local actual="${archivosnombre[$i]}"
        local Ruta="${archivosruta[$i]}"
        local Tamano="${archivostamano[$i]}"
        
        if [ "$argumentol" -eq 1 ]; then
            if [ "$actual" = "$Anterior" ] && [ "$Tamano" -gt "$argumentos" ]; then
                echo "Se ha encontrado un duplicado: $actual"
                echo "  Archivo 1: $Anterior, Ruta: $RutaAnterior"
                echo "  Archivo 2: $actual, Ruta: $Ruta"
            fi

        elif [ "$argumentol" -eq 2 ]; then
            if [ "$actual" = "$Anterior" ] && [ "$Tamano" -eq "$tamanoanterior" ] && [ "$Tamano" -gt "$argumentos" ]; then
                echo "Se ha encontrado un duplicado: $actual"
                echo "  Archivo 1: $Anterior, Ruta: $RutaAnterior, Tamaño: $tamanoanterior bytes"
                echo "  Archivo 2: $actual, Ruta: $Ruta, Tamaño: $Tamano bytes"
            fi
        else
            echo "Solo se admiten valores entre 1 y 2"
        fi

        Anterior="$actual"
        RutaAnterior="$Ruta"
        tamanoanterior="$Tamano"
    done
}

for arg in "$@"; do 
Carpeta="$arg"

       if [ -n "$espera_argumento" ]; then
           
            
            case "$espera_argumento" in

                 -l)
                       argumentol="$arg"
                       ;;
                 -p)
                       argumentop="$arg"
                        ;;
                 -s)
                        argumentos="$arg"
                        ;;
                  
            esac
            espera_argumento=""
            continue
       fi
       case "$arg" in
      
                  -l)
                        
                        espera_argumento="-l"
                        ;;
                  -p)   
                        
                        espera_argumento="-p"
                        ;;
                  -s)
                        
                        espera_argumento="-s"
                        ;;
                   *)  
                        carpetaBoolean=true
                        buscar "$Carpeta" 0
                       
                        echo "Nivel de precisión: $argumentol"
                        echo "Profundidad: $argumentop"
                        echo "Tamaño mínimo de bits: $argumentos"
                        Comparacion
                       
 
                        
                     
                        
         esac
done 
 if [ "$carpetaBoolean" = false ]; then
      buscar "$PWD" 0       
  fi
   
