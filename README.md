# Pokeemerald
Repositorio para experimentos con pokeemerald.

## Estructura
Para mantener el orden todas las adiciones se crean en la carpeta src/addons, por ejemplo el código del Pokémon Follower estaría en src/addons/follower.

## Adiciones
### Pokémon Follower
Se ha empezado a trabajar en el Pokémon Follower.
El Follower tiene una ID de evento asignada así que debería de poder moverse con un applymovement.
#### Cosas hechas
* El seguidor aparece una vez tengas un Pokémon válido y des un paso.
* Sigue al personaje y cambia su velocidad cuando es necesario.
* El personaje tiene que esperar a que el seguidor termine su animación para poder continuar, por ejemplo al saltar por los desniveles.
* Al interactuar con el seguidor se ejecuta un script, temporalmente asignado al PC.
* El seguidor es traspasable por el jugador pero no por los demás eventos.
#### Cosas por hacer
* Retocar el sistema que transforma la velocidad del siguiente movimiento.
* Añadir la habilidad de ocultar al seguidor y hacerlo automáticamente cuando vayamos a surfear o nos montemos en la bicicleta.

### Expansión de overworlds
Aun en un estado muy inicial, conseguí expandir los overworlds inicialmente añadiendo un nuevo valor de 4 bits a EventObject y EventObjectTemplate que sirviese como tabla (a lo JPAN), de forma que podía tener 4080 overworlds.

Aunque funcionaba, me gustaría modificar el valor graphicsId de las dos estructuras anteriores y que fuese un valor de 16 bits en lugar de 8 para no tener que estar comprobando la tabla.

Hice algún avance sobre esto modificando también macros/asm/map.inc y cambiando el macro de event_object de esta forma:

```assembly
	.macro object_event index:req, gfx:req, replacement:req, x:req, y:req, elevation:req, movement_type:req, x_radius:req, y_radius:req, trainer_type:req, sight_radius_tree_etc:req, script:req, event_flag:req
	.byte \index,
	.2byte \gfx
	.byte \replacement, 0
	.2byte \x
	.2byte \y
	.byte \elevation, \movement_type, ((\y_radius << 4) | \x_radius), 0
	.2byte \trainer_type, \sight_radius_tree_etc
	.4byte \script
	.2byte \event_flag
	.2byte 0
	inc _num_npcs
	.endm
```

Con esta modificación el juego funciona cuando se crea una nueva partida (no se puede utilizar la antigua ya que el tamaño del SaveBlock1 cambia) pero se ve que hay algo que sigue causando overflow en algún sitio y el juego deja de responder incluso si únicamente le añado 1 bit más (512 overworlds)

