# Application "Projet" controle d'un robot mobile 

## Text overview

```
 teta(k)                                       ---
 ---       --- consigne vitesse               | o |
|   |     | c | |           ur      wr        | d |
| p |     | i |---(X)---[Reg]---[Mot]---[Rr>--| o |-> x
| i |-wc->| n |    |                  |       | m |
| l |     | e |    --------------------       | e |-> y
| o |     | m |             ul      wl        | t |
| t |-vc->| a |---(X)---[Reg]---[Mot]---[Rr>--| r |-> teta
| e |     | t |    |                  |       | i |
|   |     | i |    --------------------       | e |
 ---       ---                                 ---
```

- Reg : régime
- Mot : moteur
- Rr : rayon roue
- Cinemati : Cinematique

## Image overview

![Project Image Overview](./doc/img/overview.png?raw=true "Project")

## Struct :
- ./bin : executable
- ./header : contains the h files
- ./src : contains the c files

## Commit convention
- BREAKING CHANGE:
- build: Changes that affect the build system or external dependencies (example scopes: gulp, broccoli, npm)
- ci: Changes to our CI configuration files and scripts (examples: CircleCi, SauceLabs)
- docs: Documentation only changes
- feat: A new feature
- fix: A bug fix
- perf: A code change that improves performance
- refactor: A code change that neither fixes a bug nor adds a feature
- test: Adding missing tests or correcting existing tests