# Secreto compartido con esteganografía

Integrantes:
- Margossian, Gabriel Viken 59130
- Bilevich, Andrés Leonardo 59108
- Lin, Scott 59339

# Descripción

Implementación de secreto compartido con esteganografía hecho en C para esconder una imagen de formato `.bmp` dentro de otras imágenes del mismo formato de forma tal que pueda ser recuperarla. Esta implementación está basada en el algoritmo de Imagen Secreta Compartida descripto en el documento `Sistema de Imagen Secreta Compartida con Optimización de la Carga Útil.pdf` que se encuentra en el repositorio. 

# Requerimientos
Libreria para hacer unit testing en C: `Check`
```bash
$> sudo apt install check
```

# Compilación
En la carpeta del proyecto ejecutar:
```bash
$> make all
```
En caso de no tener la libreria `Check` de unit testing ejecutar:
```bash
$> make main
```
Esto creará el ejecutable `main`
# Ejecución
El programa debe ser ejecutado con los siguientes argumentos:
- `d` o `r`: 
	- `d`: indica que se va a distribuir una imagen secreta en otras imágenes.
	- `r`: indica que se va a recuperar una imagen secreta a partir de otras imágenes.
- `pathImagen`: Corresponde al path de un archivo de extensión .bmp. En el caso de que se haya elegido la opción 'd' este archivo debe existir ya que es la imagen a ocultar y debe ser una imagen en blanco y negro (8 bits por pixel). Si se eligió la opción 'r' este archivo será el archivo de salida (puede no existir preveo a la ejecución del programa), con la imagen secreta revelada al finalizar el programa.   
- `k`: El número corresponde a la cantidad mínima de sombras necesarias para recuperar el secreto en un esquema (k, n). Puede valer 4,5 o 6.
- `directorio`: El path al directorio donde se encuentran las imágenes en las que se distribuirá el secreto (en el caso de que se haya elegido la opción `d`), o donde están las imágenes que contienen oculto el secreto ( en el caso de que se haya elegido la opción `r`). Debe contener imágenes de extensión `.bmp`, de 8 bits por pixel, de igual tamaño que la imagen secreta y por lo menos k imágenes en el directorio. 

## Ejemplo de ejecución
Para distribuir el secreto:
```bash
$> ./main d images/Albert.bmp 4 shades/
```
Donde `images/Albert.bmp` es la imagen que se quiere esconder y `shades/` es la carpeta en la que se encuentran las imágenes en las que se distribuirá el secreto.

Para recuperar el secreto:
```bash
$> ./main d output/secret.bmp 4 shades/
```
Donde `output/secret.bmp` es la imagen secreta y `shades/` es la carpeta en la que se encuentran las imágenes en las que se distribuyó el secreto.

