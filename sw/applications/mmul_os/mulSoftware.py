
def crear_matriz(filas, columnas):
    matriz = [[i*columnas + j + 1 for j in range(columnas)] for i in range(filas)]
    return matriz

def mostrar_matriz(matriz):
    for fila in matriz:
        print(fila)

def multiplicar_matrices(matriz_a, matriz_b):
    resultado = [[0 for _ in range(len(matriz_b[0]))] for _ in range(len(matriz_a))]

    for i in range(len(matriz_a)):
        for j in range(len(matriz_b[0])):
            for k in range(len(matriz_b)):
                resultado[i][j] += matriz_a[i][k] * matriz_b[k][j]

    return resultado


def main():
    filas_a = int(input("Ingrese el número de filas para la matriz A: "))
    columnas_a = int(input("Ingrese el número de columnas para la matriz A: "))

    filas_b = int(input("Ingrese el número de filas para la matriz B: "))
    columnas_b = int(input("Ingrese el número de columnas para la matriz B: "))

    if columnas_a != filas_b:
        print("No se pueden multiplicar matrices con estas dimensiones.")
        return

    # Crear matrices A y B
    matriz_a = crear_matriz(filas_a, columnas_a)
    matriz_b = crear_matriz(filas_b, columnas_b)

    # Mostrar matrices A y B
    #print("\nMatriz A:")
    #mostrar_matriz(matriz_a)

    #print("\nMatriz B:")
    #mostrar_matriz(matriz_b)

    # Multiplicar matrices A y B
    matriz_resultado = multiplicar_matrices(matriz_a, matriz_b)

    # Mostrar resultado
    print("\nResultado de la multiplicación:")
    mostrar_matriz(matriz_resultado)

if __name__ == "__main__":
    main()
