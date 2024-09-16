## **Tema 1 IOCLA**
---

Rezolvarea temei a durat aproximativ 2 zile.

Nu am creat fisiere *.c* sau *.h* in plus, declarand in schimb antetul functiei
get_operations in *main.c*:
```
void get_operations(void **operations);
```
 > operations = vectorul de pointeri la functiile din *operations.c*
```    
static void (*operations[8])(void *);
get_operations((void **)operations);
```
Fisierul binar *.dat* este primit ca argumentul cu numarul 1.
Dupa alocarile dinamice necesare, se citesc datele despre senzori, verificand
tipul fiecaruia.
Am utilizat vectorul de tip sensor * `aux` in care am introdus senzorii sortati
conform enuntului, pastrandu-se ordinea celor de tip PMU, respectiv TIRE.

Comenzile (`print/analyze/clear`) le citesc pe rand in sirul de caractere `s`.

1. **Print**

> Se afiseza informatiile despre senzori conform specificatiilor din enunt, 
> tratand separat cele doua tipuri de senzori.

2. **Analyze**
   
> Se parcurge vectorul de operatii al senzorului `i` si se executa operatiile
> apelandu-se prin intermediul vectorului de pointeri la functiile din
> *operations.c*.

3. **Clear**
   
> Am ultilizat variabila `ok`, care marcheaza pentru fiecare senzor `k` daca are
>  (`ok = 1`) sau nu (`ok = 0`) valori valide. Daca senzorul are valori
> invalide, mai intai se elibereaza campurile sale de `sensors_data` si
> `operations_idxs`. Senzorii de dupa acesta vor fi mutati cu o pozitie
> la stanga. Dupa aceasta se realoca vectorul de senzori cu noua lungime:
> ```
> nr_sensors--;
> v = (sensor *)realloc(v, nr_sensors * sizeof(sensor));
>```

La sfarsitul programului se elibereaza restul de memorie: componentele alocate
ale vectorului si, intr-un final, vectorul de senzori `v`.
