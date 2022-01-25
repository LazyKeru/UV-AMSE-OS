# FiltrePremierOrdre

## Compile FiltrePremierOrdre:

```
gcc ./src/FiltrePremierOrdre.c -o ./bin/FiltrePremierOrdre
```

## Run FiltrePremierOrdre :

```
./bin/FiltrePremierOrdre <gain> <cste de temps> <periode d'echantillonnage>
```

example

```
./bin/FiltrePremierOrdre 1.0 0.1 0.01
```

# WaveGenerator :

## Compile WaveGenerator:

```
gcc ./src/WaveGenerator.c -lm -o ./bin/WaveGenerator
```

## Run WaveGenerator :

```
./bin/WaveGenerator <amplitude> <frequence (Hz)> <duree (s)> <periode d'echantillonnage>
```

example

```
./bin/WaveGenerator 10 60 0.1 0.001
```

# AddNoise :

## Compile AddNoise:

```
gcc ./src/AddNoise.c -o ./bin/AddNoise
```

## Run AddNoise :

```
./bin/AddNoise <amplitude>
```

example

```
./bin/AddNoise 0.1
```

# Running as a pipeline

## First we analyze the WaveGenerator output

```
./bin/WaveGenerator 1 2 10 0.1 > ./bin/WaveGenerator.output
```

## We begin making the pipeline

```
./bin/WaveGenerator 10 60 0.1 0.001 | ./bin/AddNoise 0.1 | ./bin/FiltrePremierOrdre 1.0 0.1 0.001 | ./bin/OutputFormatter
```
