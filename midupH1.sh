#!/bin/bash
argumentol=0
argumentop=0
argumentos=0
archivosnombre=()
archivosruta=()
aux=()
espera_argumento=""
for arg in "$@"; do 
Carpeta=$1

       if [ -n "$espera_argumento" ]; then
           echo "la opción $espera_argumento espera un argumento"
            
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
                        if [ -d "$arg" ]; then
                              for archivo in "$arg"/*; do
                                    if [ -f "$archivo" ]; then
                                          nombre=$(basename "$archivo")
                                          tamano=$(stat -c%s "$archivo")
                                          ruta=$(realpath "$archivo")
                                          archivosnombre+=("$nombre")
                                          archivosruta+=("$ruta")
                                        

                                    fi
                              done
                        fi 
                        echo "Nivel de precisión: $argumentol"
                        echo "Profundidad: $argumentop"
                        echo "Tamaño mínimo de bits: $argumentos"
                        
                        for i in "${!archivosnombre[@]}"; do
                            aux+=("${archivosnombre[$i]} ${archivosruta[$i]}")
                        done
                         for entrada in "${aux[@]}"; do
                             printf "%s\n" "$entrada"
                         done | sort | column -t
                         
   
         esac
done 


