//Ignacio Melendo y Roberto Ganovic

//Librerias
#include<iostream>
#include<string>
#include<ctime>
#include<fstream>
#include<iomanip>


using namespace std;

//Constantes

const int ANCHO_PISTA = 7;
const int LARGO_PISTA = 3;
const int LIM_INF_HAB = 1;
const int LIM_SUP_HAB = 4;
const int LIM_INF_VEL = 1;
const int LIM_SUP_VEL = 4;
const bool JUEGO_ALEATORIO = true;
const bool MODO_DEBUG = false;
const int DIM_ARRAY_GOLPEOS = ANCHO_PISTA + 1;
const int DIM_ARRAY_TENISTAS = 10 +1;

//Estructuras

typedef enum { NADIE, TENISTA1, TENISTA2 } tTenista;
typedef enum { NADA, QUINCE, TREINTA, CUARENTA, VENTAJA }tPuntosJuego;
typedef int tConteoGolpes[DIM_ARRAY_GOLPEOS];

typedef struct {
	int posicion;
	tPuntosJuego puntos;
	int juegos;
	int golpes_ganadores;
	tConteoGolpes golpeos;
} tDatosPartido;

typedef struct {
	string iniciales;
	int habilidad;
	int velocidad;
	tDatosPartido datos_partido;
	int partidos_ganados;
	int partidos_perdidos;
} tDatosTenista;

typedef tDatosTenista tArrayTenistas[DIM_ARRAY_TENISTAS];

typedef struct tListaTenistas {
	tArrayTenistas arrayT;
	int contador = 0;
};



//Declaración de funciones

int  golpeoBola(int  posicion_tenista, int  habilidad);
//Mediante la posicion del tenista y su habilidad se determina en que posicion llegara la bola

int correTenista(int posicion_tenista, int velocidad, int posicion_bola);
//Dada la posicion del tenista, su habilidad y la posicion a la que va la bola devuelve un entero con la posición a la que llega el tenista que corre hacia la bola

int d7();
//Numero aleatorio entre 1 y 7

int d100();
//Numero aleatorio entre 1 y 100

void introducirTenista(string& iniciales, int& habilidad, int& velocidad);
//pide al usuario todos los datos del tenista: iniciales del nombre (3 letras, ni más ni menos), habilidad y velocidad devolviendo los tres valores.

tTenista actualizarMarcador(tTenista ganador_punto, tPuntosJuego& puntos1, tPuntosJuego& puntos2, int& juegos1, int& juegos2);
//dado el tenista ganador del punto actualiza las puntuaciones de los jugadores(sus puntos y sus juegos).En el caso deque un jugador haya ganado el juego devuelve el tenista ganador(TENISTA1 o TENISTA2);en otro caso devuelve NADIE

void pintarMarcador(string nombre1, string nombre2, tPuntosJuego puntos1, tPuntosJuego puntos2, int juegos1, int juegos2, tTenista servicio_para);
//muestra en la consola el marcador

void pintarPeloteo(string nombre1, string nombre2, int pos_t1, int pos_t2, tTenista bola_jugador, int pos_bola);
//dados  los  nombres  de  los  tenistas,  sus posiciones, el jugador que tiene la bola y la posición de ésta, dibuja el campo en la consola

tTenista lance(tTenista bola_para, tDatosTenista& tenista_golpea, tDatosTenista& tenista_recibe, int& pos_bola);
//Dado  el  tenista  que  golpea  la  bola,  su  nombre  y  su  habilidad,  la velocidad del que recibe la bola y su posición, y la posición de la bola, realiza el golpe dela bola usando golpeBola(), actualizando la posición de esta.

tTenista jugarPunto(tDatosTenista& tenista1, tDatosTenista& tenista2, tTenista servicio_para);
//dados los datos de los tenistas y el tenista que sirve(servicio), conduce el juego de un punto,colocando a los jugadores y la bola en el centro del campo, pintando el campo y, mientrasno  haya  ganador  del  punto, realizando  un  lance()  del  partido  e  intercambiando  quiéngolpea la bola tras el lance.Además, llama a pintarPeloteo() para mostrar gráficamenteel estado del partido.La función devuelve el ganador del punto(TENISTA1 o TENISTA2).

tTenista jugarJuego(tDatosTenista& tenista1, tDatosTenista& tenista2, tTenista servicio_para);
// dados  los  datos  de  los  tenistas  y  el  que  sirve (servicio), conduce  un  juego  del  set.Mientras  no  haya  ganador  del  juego, muestra  elmarcador  con  pintarMarcador()  y  llama  a  jugarPunto().Al  acabar  el  punto, debemostrar quién ha sido el ganador del punto y actualizar el marcador usandoactualizarMarcador().La función devuelve el jugador que gana el juego.

tPuntosJuego numAPunt(int punto);
//Transforma int a tPuntosJuego 

int puntosANum(tPuntosJuego punto);
//Transforma tPuntosJuego a int

string puntosAString(tPuntosJuego punto);
//Transoforma tPuntosJuego a string

tTenista primerServicio();
//Devuelve aleatoriamente un tenista

void alternarSaque(tTenista& servicio);
//Cambia de tenista cuando se acaba cada juego

int dx(int x);
//Dado de x posibilidades

void estadisticas(string nombre, tConteoGolpes golpes, int golpes_ganados);
//Imprime las estadisticas de los jugadores

void cargar(tListaTenistas& listaT);
//Carga los datos del txt en las variables

void guardar(const tListaTenistas& listaT);
//Recibe una lista de tenistas y lo guarda en el txt

void mostrar(const tListaTenistas& listaT);
//Recibe una lista de tenista y la muestra por consola en formato tabla

void mostrarIniciales(const tListaTenistas& listaT);
//Muestra en pantalla las iniciales de todos los tenistas

int buscarIniciales(const tListaTenistas& listaT, string ini);
//Recibe unas iniciales, y devuelve la posicion donde se encuentra el tenista con dichas iniciales o -1 si no hay ninguno

void eliminarTenista(tListaTenistas& listaT);
//Mediante sus iniciales se busca al tenista en la lista y si lo encuentra lo elimina

void introducirTenista(tListaTenistas& listaT);
//Se permitira al usuario introducir un nuevo tenista en la lista si existe hueco para ello. Los datos del nuevo tenista se le pedirán al usuario por teclado, igual que en la version 2

tTenista jugarPartido(tDatosTenista& tenista1, tDatosTenista& tenista2);
//Se juega el partido, devolvera los tenistas actualizados e indicara cual de ellos gano el partido

void mostrarMenu();
//Muestra en pantalla el menu del juego

void casoCuatro(tListaTenistas& listaT);
//Cuarta opcion del menu principal

void casoCinco(tListaTenistas& listaT);
//Cuarta opcion del menu principal

void casoSeis(tListaTenistas& listaT);
//Cuarta opcion del menu principal

void jugarTorneo(tDatosTenista& tenista1, tDatosTenista& tenista2, tDatosTenista& tenista3, tDatosTenista& tenista4);
//Juega el torneo entre los cuatro tenistas seleccionados y devuelve las iniciales del tenista ganador del torneo



//Main

int main() {

	srand((unsigned int)time(NULL)); //Para que el aleatorio funcione bien

	tListaTenistas listaT; //Declaramos el tListaTenista
	bool fin = false;
	int opcion;

	cargar(listaT); //Cargamos los datos del .txt

	while (!fin) {//Bucle del menu de modo

		mostrarMenu();

		cout << "Opcion:";
		cin >> opcion;
		while (opcion < 0 || opcion >6) {
			cout << "No existe esa opcion vuelva a introducirla";
			cin >> opcion;
		}

		switch (opcion) {
		case 0://Salimos y guardamos los datos
			fin = true;
			guardar(listaT);
			break;
		case 1://Mostramos los datos de los tenistas

			mostrar(listaT);

			break;
		case 2: //Añadimos un nuevo tenista

			introducirTenista(listaT);

			break;
		case 3: //Eliminamos un tenista

			eliminarTenista(listaT);

			break;
		case 4://Partido 1v1

			casoCuatro(listaT);

			break;
		case 5://Torneo seleccion

			casoCinco(listaT);
		
			break;
		case 6://Torneo top 4

			casoSeis(listaT);

			break;
		}

	}

	return 0;
}

//Funciones

int  golpeoBola(int  posicion_tenista, int  habilidad) {
	int calle, distancia, acierto, destino, resultado;

	if (JUEGO_ALEATORIO) { //Si es aleatorio se usa el d7 para seleccionar una calle
		calle = dx(ANCHO_PISTA);
	}
	else {
		cout << "\nSelecciona la calle a la que golpear la bola: " << endl;
		cin >> calle;
		while (calle > ANCHO_PISTA || calle < 1) {
			cout << "Selecciona la calle a la que golpear la bola: " << endl; //Pregunta hasta que se selecciona una calle entre 1 y 7
			cin >> calle;
		}
	}

	if (MODO_DEBUG) { //DEBUG
		cout << "El jugador dispara hacia la calle " << calle << endl;
	}
	calle -= 1; //Calle = 0 <= num <= 6
	if (calle <= posicion_tenista) { //Se calcula la distancia con una resta
		distancia = posicion_tenista - calle;
	}
	else {
		distancia = calle - posicion_tenista;
	}

	if (distancia > habilidad) { //Se lleva a cabo la ecuacion si la habilidad no supera la distancia
		acierto = 100 - (((distancia - habilidad) * 100) / ((ANCHO_PISTA - 1) - LIM_INF_HAB));
		resultado = d100() - 1;
		if (resultado < acierto) {
			destino = calle;
			if (MODO_DEBUG) { //DEBUG
				cout << "Tiro complicado que... (probab_exito = " << acierto << " y resultado = " << resultado << ") Llega a su destino!" << endl;
			}
		}
		else {
			if (d100() < 50) {
				destino = calle - 1;
			}
			else {
				destino = calle + 1;
			}
			if (MODO_DEBUG) { //DEBUG
				cout << "Tiro complicado que... (probab_exito = " << acierto << " y resultado = " << resultado << ") No ha sido preciso!" << endl;
			}
		}
	}
	else { // Se acierta sin hacer la funcion
		destino = calle;
		if (MODO_DEBUG) { //DEBUG
			cout << "Ese ha sido un tiro sencillo!" << endl;
		}
	}
	if (MODO_DEBUG) { //DEBUG
		cout << "La bola llega a la casilla " << destino + 1 << endl;
	}

	return destino;
}

int correTenista(int posicion_tenista, int velocidad, int posicion_bola) {
	int  distancia;
	bool sum = false;

	if (posicion_bola <= posicion_tenista) { // Halla la distancia y a que lado del tenista se encuentra (con el bool sum)
		distancia = posicion_tenista - posicion_bola;
	}
	else {
		distancia = posicion_bola - posicion_tenista;
		sum = true;
	}

	if (velocidad < distancia) { //Si no llega
		if (sum) {
			posicion_tenista += velocidad;
		}
		else {
			posicion_tenista -= velocidad;
		}
		if (MODO_DEBUG) {
			cout << "\nSu rival no llega a la bola." << endl;
		}
	}
	else { //Si llega
		posicion_tenista = posicion_bola;
		if (MODO_DEBUG) { //DEBUG
			cout << "\nSu rival llega a la bola." << endl;
		}
	}
	return posicion_tenista;
}

int d7() {
	int resultado;

	resultado = (rand() % 7) + 1; //Dado aleatorio del 1 al 7

	return resultado;
}

int d100() {
	int resultado;

	resultado = (rand() % 100) + 1; //Dado aleatorio del 1 al 100

	return resultado;
}

void introducirTenista(string& iniciales, int& habilidad, int& velocidad) {
	cout << "   -Introduce sus iniciales (3 letras): ";
	cin >> iniciales;

	while (iniciales.length() != 3) {
		cout << "   -Por favor introduzca tres iniciales, ni mas ni menos: ";
		cin >> iniciales;
	}

	cout << "   -Introduzca su habilidad (Valor entre " << LIM_INF_HAB << " y " << LIM_SUP_HAB << "): ";
	cin >> habilidad;

	while (habilidad < LIM_INF_HAB || habilidad > LIM_SUP_HAB) {
		cout << "   -Por favor introduzca un valor permitido, Habilidad (Valor entre " << LIM_INF_HAB << " y " << LIM_SUP_HAB << "): ";
		cin >> habilidad;
	}

	cout << "   -Introduzca su velocidad (Valor entre " << LIM_INF_VEL << " y " << LIM_SUP_VEL << "): ";
	cin >> velocidad;

	while (velocidad < LIM_INF_VEL || velocidad > LIM_SUP_VEL) {
		cout << "   -Por favor introduzca un valor permitido, (Valor entre " << LIM_INF_VEL << " y " << LIM_SUP_VEL << "): ";
		cin >> velocidad;
	}
}

tTenista actualizarMarcador(tTenista ganador_punto, tPuntosJuego& puntos1, tPuntosJuego& puntos2, int& juegos1, int& juegos2) {
	int punt1, punt2;
	tTenista ganador = NADIE;

	punt1 = puntosANum(puntos1);
	punt2 = puntosANum(puntos2);

	if (ganador_punto == TENISTA1) {

		punt1++;

		if (punt1 == 4 && punt2 == 4) {
			punt1 = 3;
			punt2 = 3;
		}
		if ((punt1 == 4 && punt2 < 3) || punt1 == 5) {
			juegos1++;
			ganador = TENISTA1;
			puntos1 = NADA;
			puntos2 = NADA;
		}
	}
	else if (ganador_punto == TENISTA2) {
		punt2++;

		if (punt1 == 4 && punt2 == 4) {
			punt1 = 3;
			punt2 = 3;
		}
		if ((punt2 == 4 && punt1 < 3) || punt2 == 5) {
			juegos2++;
			ganador = TENISTA2;
			puntos1 = NADA;
			puntos2 = NADA;
		}
	}
	puntos1 = numAPunt(punt1);
	puntos2 = numAPunt(punt2);

	return ganador;
}

void pintarMarcador(string nombre1, string nombre2, tPuntosJuego puntos1, tPuntosJuego puntos2, int juegos1, int juegos2, tTenista servicio_para) {

	switch (servicio_para) {

	case NADIE:
		cout << endl << nombre1 << " " << juegos1 << " : " << puntosAString(puntos1) << endl;
		cout << nombre2 << " " << juegos2 << " : " << puntosAString(puntos2) << endl << endl;
		break;

	case TENISTA1:
		cout << endl << nombre1 << " " << juegos1 << " : " << puntosAString(puntos1) << " *" << endl;
		cout << nombre2 << " " << juegos2 << " : " << puntosAString(puntos2) << endl << endl;
		break;

	case TENISTA2:
		cout << endl << nombre1 << " " << juegos1 << " : " << puntosAString(puntos1) << endl;
		cout << nombre2 << " " << juegos2 << " : " << puntosAString(puntos2) << " *" << endl << endl;
		break;
	}
}

void pintarPeloteo(string nombre1, string nombre2, int pos_t1, int pos_t2, tTenista bola_jugador, int pos_bola) {

	pos_bola += 1;
	pos_t1 += 1;
	pos_t2 += 1;
	//Linea nombres
	cout << " ";
	for (int i = 1; i < pos_t1; i++) {
		cout << "  ";
	}
	cout << nombre1 << endl;

	//Linea1 
	cout << " ";
	for (int i = 0; i < ANCHO_PISTA; i++) {
		cout << " -";
	}
	cout << endl;

	//Linea2

	if (pos_bola == 0 && bola_jugador == TENISTA1) {
		cout << "o";
	}
	else {
		cout << " ";

	}
	for (int i = 0; i < ANCHO_PISTA + 1; i++) {

		cout << "|";
		if (pos_bola == i + 1 && bola_jugador == TENISTA1) {
			cout << "o";
		}
		else {
			cout << " ";

		}
	}
	cout << "\n";

	//LineaLargo
	for (int i = 1; i < LARGO_PISTA; i++) {
		cout << " |";
		for (int i = 0; i < ANCHO_PISTA; i++) {
			cout << " |";
		}
		cout << "\n";
	}

	//LineaMedio
	cout << "--";
	for (int i = 0; i < ANCHO_PISTA; i++) {
		cout << i + 1 << "-";
	}
	cout << "-\n";

	//LineaLargo
	for (int i = 1; i < LARGO_PISTA; i++) {
		cout << " |";
		for (int i = 0; i < ANCHO_PISTA; i++) {
			cout << " |";
		}
		cout << "\n";
	}

	//Linea8

	if (pos_bola == 0 && bola_jugador == TENISTA2) {
		cout << "o";
	}
	else {
		cout << " ";

	}
	for (int i = 0; i < ANCHO_PISTA + 1; i++) {

		cout << "|";
		if (pos_bola == i + 1 && bola_jugador == TENISTA2) {
			cout << "o";
		}
		else {
			cout << " ";

		}
	}
	cout << "\n";

	//Linea9
	cout << " ";
	for (int i = 0; i < ANCHO_PISTA; i++) {
		cout << " -";
	}
	cout << endl;

	//LineaNombres
	cout << " ";
	for (int i = 1; i < pos_t2; i++) {
		cout << "  ";
	}
	cout << nombre2 << endl << endl;
}

tTenista lance(tTenista bola_para, tDatosTenista& tenista_golpea, tDatosTenista& tenista_recibe, int& pos_bola) {
	int destino, corredor;
	tTenista ganador = NADIE;

	cout << "Golpea " << tenista_golpea.iniciales << endl << endl;
	destino = golpeoBola(pos_bola, tenista_golpea.habilidad);
	tenista_golpea.datos_partido.golpeos[destino + 1]++;

	if (destino == -1 || destino == ANCHO_PISTA) { //SI FALLA EL TIRO
		ganador = bola_para;
		pos_bola = destino;
	}
	else { //NO FALLA
		corredor = correTenista(tenista_recibe.datos_partido.posicion, tenista_recibe.velocidad, destino);
		if (corredor != destino) {
			if (bola_para == TENISTA1) { //NO LLEGA
				ganador = TENISTA2;
			}
			else {
				ganador = TENISTA1;
			}
			tenista_golpea.datos_partido.golpes_ganadores++;
		}
		tenista_recibe.datos_partido.posicion = corredor;
		pos_bola = destino;
	}
	return ganador;
}

tTenista jugarPunto(tDatosTenista& tenista1, tDatosTenista& tenista2, tTenista servicio_para) {

	bool puntoMarcado = false;
	tTenista jugador1 = TENISTA1, jugador2 = TENISTA2, ganador;
	int bola, pos_inicio = (ANCHO_PISTA + 1) / 2;
	tenista1.datos_partido.posicion = pos_inicio - 1;
	tenista2.datos_partido.posicion = pos_inicio - 1;
	bola = pos_inicio - 1;


	if (servicio_para == jugador1) { //servicio jug1

		system("pause");
		pintarPeloteo(tenista1.iniciales, tenista2.iniciales, tenista1.datos_partido.posicion, tenista2.datos_partido.posicion, jugador1, bola);

		while (!puntoMarcado) {
			system("pause");
			ganador = lance(jugador2, tenista1, tenista2, bola); //saca //devuelve
			pintarPeloteo(tenista1.iniciales, tenista2.iniciales, tenista1.datos_partido.posicion, tenista2.datos_partido.posicion, jugador2, bola);
			if (ganador != NADIE) { //Si marca alguien
				puntoMarcado = true;
			}
			else
			{
				system("pause");
				ganador = lance(jugador1, tenista2, tenista1, bola); //devuelve
				if (ganador != NADIE) { //Si marca alguien
					puntoMarcado = true;
				}
				pintarPeloteo(tenista1.iniciales, tenista2.iniciales, tenista1.datos_partido.posicion, tenista2.datos_partido.posicion, jugador1, bola);

			}

		}
	}
	else { //servicio jug2
		pintarPeloteo(tenista1.iniciales, tenista2.iniciales, tenista1.datos_partido.posicion, tenista2.datos_partido.posicion, jugador2, bola);
		while (!puntoMarcado) {
			system("pause");
			ganador = lance(jugador1, tenista2, tenista1, bola); //saca //devuelve
			pintarPeloteo(tenista1.iniciales, tenista2.iniciales, tenista1.datos_partido.posicion, tenista2.datos_partido.posicion, jugador1, bola);
			if (ganador != NADIE) { //Si marca alguien
				puntoMarcado = true;
			}
			else
			{
				system("pause");
				ganador = lance(jugador2, tenista1, tenista2, bola); //devuelve
				if (ganador != NADIE) { //Si marca alguien
					puntoMarcado = true;
				}
				pintarPeloteo(tenista1.iniciales, tenista2.iniciales, tenista1.datos_partido.posicion, tenista2.datos_partido.posicion, jugador2, bola);
			}
		}
	}
	return ganador;
}

tTenista jugarJuego(tDatosTenista& tenista1, tDatosTenista& tenista2, tTenista servicio_para) {
	tTenista ganadorJuego = NADIE;
	bool acabaJuego = false;
	tTenista ganadorPunto = NADIE;
	tenista1.datos_partido.puntos = NADA;
	tenista2.datos_partido.puntos = NADA;


	if (servicio_para == TENISTA1) {
		cout << endl << "Servicio para " << tenista1.iniciales << endl;
	}
	if (servicio_para == TENISTA2) {
		cout << endl << "Servicio para " << tenista2.iniciales << endl;
	}
	while (!acabaJuego) {
		pintarMarcador(tenista1.iniciales, tenista2.iniciales, tenista1.datos_partido.puntos, tenista2.datos_partido.puntos, tenista1.datos_partido.juegos, tenista2.datos_partido.juegos, servicio_para);
		ganadorPunto = jugarPunto( tenista1, tenista2, servicio_para );
		if (ganadorPunto == TENISTA1) {
			cout << "El ganador del punto es " << tenista1.iniciales << endl;
		}
		else if (ganadorPunto == TENISTA2) {
			cout << "El ganador del punto es " << tenista2.iniciales << endl;
		}
		ganadorJuego = actualizarMarcador(ganadorPunto, tenista1.datos_partido.puntos, tenista2.datos_partido.puntos, tenista1.datos_partido.juegos, tenista2.datos_partido.juegos);

		if (ganadorJuego != NADIE) {
			//estadisticas
			acabaJuego = true;
			estadisticas(tenista1.iniciales, tenista1.datos_partido.golpeos, tenista1.datos_partido.golpes_ganadores);
			estadisticas(tenista2.iniciales, tenista2.datos_partido.golpeos, tenista2.datos_partido.golpes_ganadores);
		}

	}
	return ganadorJuego;
}

tPuntosJuego numAPunt(int punto) {
	tPuntosJuego puntuacion = NADA;

	switch (punto) { //NADA->00  QUINCE->1  TREINTA->2  CUARENTA->3  VENTAJA->4
	case 0:
		puntuacion = NADA;
		break;
	case 1:
		puntuacion = QUINCE;
		break;
	case 2:
		puntuacion = TREINTA;
		break;
	case 3:
		puntuacion = CUARENTA;
		break;
	case 4:
		puntuacion = VENTAJA;
		break;
	}

	return puntuacion;
}

int puntosANum(tPuntosJuego punto) {
	int puntuacion = 0;

	switch (punto) { //NADA->00  QUINCE->1  TREINTA->2  CUARENTA->3  VENTAJA->4
	case NADA:
		puntuacion = 0;
		break;
	case QUINCE:
		puntuacion = 1;
		break;
	case TREINTA:
		puntuacion = 2;
		break;
	case CUARENTA:
		puntuacion = 3;
		break;
	case VENTAJA:
		puntuacion = 4;
		break;
	}

	return puntuacion;
}

string puntosAString(tPuntosJuego punto) {
	string puntuacion;

	switch (punto) { //NADA->00  QUINCE->1  TREINTA->2  CUARENTA->3  VENTAJA->4
	case NADA:
		puntuacion = "00";
		break;
	case QUINCE:
		puntuacion = "15";
		break;
	case TREINTA:
		puntuacion = "30";
		break;
	case CUARENTA:
		puntuacion = "40";
		break;
	case VENTAJA:
		puntuacion = "AD";
		break;
	}

	return puntuacion;
}

tTenista primerServicio() {

	//SERVICIO ALEATORIO 

	int saque;
	tTenista servicio;

	saque = d100();

	if (saque % 2 == 0) {
		servicio = TENISTA1;

	}
	else {
		servicio = TENISTA2;
	}

	return servicio;
}

void alternarSaque(tTenista& servicio) {

	//SE VA CAMBIANDO EL SAQUE

	switch (servicio) {
	case TENISTA1:
		servicio = TENISTA2;
		break;

	case TENISTA2:
		servicio = TENISTA1;
		break;
	}
}

int dx(int x) {

	//DADO X CARAS

	int resultado;

	resultado = (rand() % x) + 1; //Dado aleatorio del 1 al x

	return resultado;
}

void estadisticas(string nombre, tConteoGolpes golpes, int golpes_ganados) {
	int golpesT = 0;

	//MENU ESTADISTICAS

	for (int i = 0; i <= DIM_ARRAY_GOLPEOS; i++) {
		golpesT += golpes[i];
	}

	cout << "Estadisticas de " << nombre << endl;
	cout << "   " << "Golpes totales: " << golpesT << endl;
	cout << "   " << "Golpes ganados: " << golpes_ganados << endl;
	cout << "   " << "Errores no forzados: " << golpes[0] + golpes[DIM_ARRAY_GOLPEOS] << endl;
	cout << "   " << "Distribucion de los golpes en la pista: " << endl;
	cout << "   " << "   " << "Calle:   ";
	for (int i = 0; i <= DIM_ARRAY_GOLPEOS; i++) {
		cout << i << "   ";
	}
	cout << endl << "   " << "   " << "%   " << "   ";

	for (int i = 0; i <= DIM_ARRAY_GOLPEOS; i++) {
		cout << (golpes[i] * 100) / golpesT << "   ";
	}
	cout << endl;
}

void cargar(tListaTenistas& listaT) {
	ifstream file;

	//CARGAMOS LOS DATOS DEL TXT A NUESTRO LISTAT 

	file.open("tenistas.txt");

	while (!file.eof()) {
		file >> listaT.arrayT[listaT.contador].iniciales >> listaT.arrayT[listaT.contador].habilidad >> listaT.arrayT[listaT.contador].velocidad >> listaT.arrayT[listaT.contador].partidos_ganados >> listaT.arrayT[listaT.contador].partidos_perdidos;

		listaT.contador++;
	}
	listaT.contador -= 1;
}

void guardar(const tListaTenistas& listaT) {
	ofstream file;
	file.open("tenistas.txt");

	//GUARDAMOS LOS DATOS EN EL .TXT

	for (int i = 0; i <= listaT.contador -1; i++) {
		file << listaT.arrayT[i].iniciales << " " << listaT.arrayT[i].habilidad << " " << listaT.arrayT[i].velocidad << " " << listaT.arrayT[i].partidos_ganados << " " << listaT.arrayT[i].partidos_perdidos<< endl;
	}
	file << listaT.arrayT[listaT.contador].iniciales << " " << listaT.arrayT[listaT.contador].habilidad << " " << listaT.arrayT[listaT.contador].velocidad << " " << listaT.arrayT[listaT.contador].partidos_ganados << " " << listaT.arrayT[listaT.contador].partidos_perdidos;
}

void mostrar(const tListaTenistas& listaT) {

	//MOSTRAMOS LAS CARACTERISTICAS DE CADA TENISTA

	cout << "  " << "INI" << "    " << "HAB" << "    " << "VEL" << "    " << "PG " << "    " << "PP " << endl;
	for (int i = 0; i < listaT.contador+1; i++) {
		cout << "  " << listaT.arrayT[i].iniciales << "    " << listaT.arrayT[i].habilidad << "      " << listaT.arrayT[i].velocidad << "      " << listaT.arrayT[i].partidos_ganados << "     " << listaT.arrayT[i].partidos_perdidos << endl;
	}
}

void mostrarIniciales(const tListaTenistas& listaT) {

	//SE MUESTRAN TODAS LAS INICIALES DE LOS TENISTAS

	cout << "Iniciales de los tenistas:   " << listaT.arrayT[0].iniciales;

	for (int i = 1; i < listaT.contador +1; i++) {
		cout << "  -  " << listaT.arrayT[i].iniciales;
	}
	cout << endl;
}

int buscarIniciales(const tListaTenistas& listaT, string ini) {
	int pos = -1;

	//BUSCAMOS EN EL ARRAY

	for (int i = 0; i <= listaT.contador; i++) {
		if (ini == listaT.arrayT[i].iniciales) {
			pos = i;
		}
	}
	return pos;
}

void eliminarTenista(tListaTenistas& listaT) {
	string inicial;
	int pos = 0;

	//SELECCIONAR TENISTA A ELIMINAR

	mostrarIniciales(listaT);
	cout << "Introduce las iniciales del tenista a eliminar: ";
	cin >> inicial;

	pos = buscarIniciales(listaT, inicial);

	//BORRAMOS SUS DATOS

	if (pos != -1) {
		cout << "Tenista eliminado" << endl;
		for (int i = pos; i < listaT.contador; i++) {
			listaT.arrayT[i].iniciales = listaT.arrayT[i + 1].iniciales;
			listaT.arrayT[i].habilidad = listaT.arrayT[i + 1].habilidad;
			listaT.arrayT[i].velocidad = listaT.arrayT[i + 1].velocidad;
			listaT.arrayT[i].partidos_ganados = listaT.arrayT[i + 1].partidos_ganados;
			listaT.arrayT[i].partidos_perdidos = listaT.arrayT[i + 1].partidos_perdidos;

		}
		listaT.contador = listaT.contador - 1;
	}
	else {
		cout << "Tenista no encontrado." << endl;
	}
}

void introducirTenista(tListaTenistas& listaT) {
	string ini;
	int hab, vel;

	//PEDIMOS LOS DATOS

	if (listaT.contador == DIM_ARRAY_TENISTAS - 1  ) {
		cout << "El limite maximo de tenistas ha sido alcanzado, por favor borre alguno" << endl;
	}
	else {
		cout << "   -Introduce sus iniciales (3 letras): ";
		cin >> ini;

		while (ini.length() != 3 || buscarIniciales(listaT, ini) != -1) {
			cout << "   -Por favor introduzca tres iniciales, ni mas ni menos (El tenista no puede ser ya existente): ";
			cin >> ini;
		}

		cout << "   -Introduzca su habilidad (Valor entre " << LIM_INF_HAB << " y " << LIM_SUP_HAB << "): ";
		cin >> hab;

		while (hab < LIM_INF_HAB || hab > LIM_SUP_HAB) {
			cout << "   -Por favor introduzca un valor permitido, Habilidad (Valor entre " << LIM_INF_HAB << " y " << LIM_SUP_HAB << "): ";
			cin >> hab;
		}

		cout << "   -Introduzca su velocidad (Valor entre " << LIM_INF_VEL << " y " << LIM_SUP_VEL << "): ";
		cin >> vel;

		while (vel < LIM_INF_VEL || vel > LIM_SUP_VEL) {
			cout << "   -Por favor introduzca un valor permitido, (Valor entre " << LIM_INF_VEL << " y " << LIM_SUP_VEL << "): ";
			cin >> vel;
		}

		//LOS GUARDAMOS

		listaT.arrayT[listaT.contador + 1].iniciales = ini;
		listaT.arrayT[listaT.contador + 1].habilidad = hab;
		listaT.arrayT[listaT.contador + 1].velocidad = vel;
		listaT.arrayT[listaT.contador + 1].partidos_ganados = 0;
		listaT.arrayT[listaT.contador + 1].partidos_perdidos = 0;
		listaT.contador++;
	}
}

tTenista jugarPartido(tDatosTenista& tenista1, tDatosTenista& tenista2) {
	tTenista ganador = NADIE;
	int pg1, pg2, pp1, pp2;
	bool victoriaSet = false;
	tTenista servicio;

	pg1 = tenista1.partidos_ganados;
	pg2 = tenista2.partidos_ganados;
	pp1 = tenista1.partidos_perdidos;
	pp2 = tenista2.partidos_perdidos;

	//SE RESTABLECEN LOS DATOS DEL PARTIDO

	for (int i = 0; i <= DIM_ARRAY_GOLPEOS; i++) {
		tenista1.datos_partido.golpeos[i] = 0;
		tenista2.datos_partido.golpeos[i] = 0;
	}
	tenista1.datos_partido.golpes_ganadores = 0;
	tenista2.datos_partido.golpes_ganadores = 0;

	tenista1.datos_partido.juegos = 0;
	tenista2.datos_partido.juegos = 0;     

	//SE ELIGE EL TENISTA QUE SACA

	servicio = primerServicio();

	//SE JUEGA EL PARTIDO

	while (!victoriaSet) {
		jugarJuego(tenista1, tenista2, servicio);
		if ((tenista1.datos_partido.juegos == 3 && tenista2.datos_partido.juegos < 2) || (tenista2.datos_partido.juegos >= 2 && (tenista1.datos_partido.juegos - tenista2.datos_partido.juegos >= 2))) {
			victoriaSet = true;
			cout << tenista1.iniciales << " " << tenista1.datos_partido.juegos << endl;
			cout << tenista2.iniciales << " " << tenista2.datos_partido.juegos << endl << endl;
			cout << tenista1.iniciales << " se hizo con el partido" << endl;
			cout << "Gracias por jugar!" << endl;
			ganador = TENISTA1;

		}
		if ((tenista2.datos_partido.juegos == 3 && tenista1.datos_partido.juegos < 2) || (tenista1.datos_partido.juegos >= 2 && (tenista2.datos_partido.juegos - tenista1.datos_partido.juegos >= 2))) {
			victoriaSet = true;
			cout << tenista1.iniciales << " " << tenista1.datos_partido.juegos << endl;
			cout << tenista2.iniciales << " " << tenista2.datos_partido.juegos << endl << endl;
			cout << tenista2.iniciales << " se hizo con el partido" << endl;
			system("pause");
			ganador = TENISTA2;
		}
		alternarSaque(servicio);
	}
	tenista1.partidos_ganados = pg1;
	tenista2.partidos_ganados = pg2;
	tenista1.partidos_perdidos = pp1;
	tenista2.partidos_perdidos = pp2;

	//SE AÑADE LOS PARTDIOS GANADOS Y PERDIDOS

	if (ganador == TENISTA1) {
		tenista1.partidos_ganados++;
		tenista2.partidos_perdidos++;
	}
	else {
		tenista2.partidos_ganados++;
		tenista1.partidos_perdidos++;
	}


	return ganador;
	
}

void mostrarMenu(){

	//MENU DE SELECCION DE MODO

	cout << "---------------------" << endl << "SIMULADOR DE TENIS V3" << endl << "---------------------"<<endl<<endl;
	cout << "<1> Ver datos tenistas" << endl;
	cout << "<2> Nuevo Tenista" << endl;
	cout << "<3> Eliminar Tenista" << endl;
	cout << "<4> Jugar partido"<<endl;
	cout << "<5> Torneos semifinales/ final" << endl;
	cout << "<6> Torneo top-4" << endl;
	cout << "<0> Salir" << endl <<endl;
}

void casoCuatro(tListaTenistas& listaT) {

	string inic1, inic2;
	int cont1, cont2;

	cout << endl;
	mostrarIniciales(listaT);

	//SELECCIONAMOS LOS DOS TENISTAS

	cout << endl << "Introduce las iniciales del tenista 1: ";
	cin >> inic1;

	cont1=buscarIniciales(listaT, inic1);
	
	while (cont1 == -1) {
		cout << "	No existe ningun tenista con esas iniciales" << endl << "Introduce las iniciales del tenista 1: ";
			cin >> inic1;
			cont1 = buscarIniciales(listaT, inic1);
	}

	cout << endl << "Introduce las iniciales del tenista 2: ";
	cin >> inic2;

	cont2 = buscarIniciales(listaT, inic2);

	while (cont2 == -1 || cont2 == cont1) {
		if (cont2 == -1) {
			cout << "	No existe ningun tenista con esas iniciales" << endl << "Introduce las iniciales del tenista 2: ";
			cin >> inic2;
			cont2 = buscarIniciales(listaT, inic2);
		}
		else {
			cout << "	Tenista ya seleccionado" << endl << "Introduce las iniciales del tenista 2: ";
			cin >> inic2;
			cont2 = buscarIniciales(listaT, inic2);
		}
	}

	//JUGAMOS PARTIDO ENTRE ELLOS

	jugarPartido(listaT.arrayT[cont1], listaT.arrayT[cont2]);

}

void casoCinco(tListaTenistas& listaT) {

	string inic1, inic2, inic3, inic4;
	int cont1, cont2, cont3, cont4;

	//SELECCIONAMOS TENISTAS

	cout << endl;
	mostrarIniciales(listaT);

	cout << endl << "Introduce las iniciales del tenista 1: ";
	cin >> inic1;

	cont1 = buscarIniciales(listaT, inic1);

	while (cont1 == -1) {
		cout << "	No existe ningun tenista con esas iniciales" << endl << "Introduce las iniciales del tenista 1: ";
		cin >> inic1;
		cont1 = buscarIniciales(listaT, inic1);
	}

	cout << endl << "Introduce las iniciales del tenista 2: ";
	cin >> inic2;

	cont2 = buscarIniciales(listaT, inic2);

	while (cont2 == -1 || cont2 == cont1) {
		if (cont2 == -1) {
			cout << "	No existe ningun tenista con esas iniciales" << endl << "Introduce las iniciales del tenista 2: ";
			cin >> inic2;
			cont2 = buscarIniciales(listaT, inic2);
		}
		else {
			cout << "	Tenista ya seleccionado" << endl << "Introduce las iniciales del tenista 2: ";
			cin >> inic2;
			cont2 = buscarIniciales(listaT, inic2);
		}
	}

	cout << endl << "Introduce las iniciales del tenista 3: ";
	cin >> inic3;

	cont3 = buscarIniciales(listaT, inic3);

	while (cont3 == -1 || cont3 == cont1 || cont3 == cont2) {
		if (cont3 == -1) {
			cout << "	No existe ningun tenista con esas iniciales" << endl << "Introduce las iniciales del tenista 3: ";
			cin >> inic3;
			cont3 = buscarIniciales(listaT, inic3);
		}
		else {
			cout << "	Tenista ya seleccionado" << endl << "Introduce las iniciales del tenista 3: ";
			cin >> inic3;
			cont3 = buscarIniciales(listaT, inic3);
		}
	}

	cout << endl << "Introduce las iniciales del tenista 4: ";
	cin >> inic4;

	cont4 = buscarIniciales(listaT, inic4);

	while (cont4 == -1 || cont4 == cont1 || cont4 == cont2 || cont4 == cont3) {
		if (cont4 == -1) {
			cout << "	No existe ningun tenista con esas iniciales" << endl << "Introduce las iniciales del tenista 4: ";
			cin >> inic4;
			cont4 = buscarIniciales(listaT, inic4);
		}
		else {
			cout << "	Tenista ya seleccionado" << endl << "Introduce las iniciales del tenista 4: ";
			cin >> inic4;
			cont4 = buscarIniciales(listaT, inic4);
		}
	}

	//JUGAMOS EL TORNEO ENTRE ELLOS

	jugarTorneo(listaT.arrayT[cont1], listaT.arrayT[cont2], listaT.arrayT[cont3], listaT.arrayT[cont4]);

}

void casoSeis(tListaTenistas& listaT) {

	//SE SELECCIONAN LOS 4 CON MAS PARTIDOS GANADOS

	int a = 0, b = 0, c = 0, d = 0;

	for (int i = 0; i <= listaT.contador; i++) { //encuentra el menor tenista y lo guarda en todas las variables
		if (listaT.arrayT[i].partidos_ganados < listaT.arrayT[a].partidos_ganados) {
			a = i;
			b = i;
			c = i;
			d = i;
		}
	}

	cout << listaT.arrayT[a].iniciales << endl;

	for (int i = 0; i <= listaT.contador; i++) { // 1
		if (listaT.arrayT[i].partidos_ganados > listaT.arrayT[a].partidos_ganados) {
			a = i;
		}
	}

	for (int i = 0; i <= listaT.contador; i++) { // 2 != 1
		if (i != a) {
			if (listaT.arrayT[i].partidos_ganados > listaT.arrayT[b].partidos_ganados) {
				b = i;
			}
		}
	}

	for (int i = 0; i <= listaT.contador; i++) {
		if (i != a && i != b) {
			if (listaT.arrayT[i].partidos_ganados > listaT.arrayT[c].partidos_ganados) {
				c = i;
			}
		}
	}

	for (int i = 0; i <= listaT.contador; i++) {
		if (i != a && i != b && i != c) {
			if (listaT.arrayT[i].partidos_ganados > listaT.arrayT[d].partidos_ganados) {
				d = i;
			}
		}
	}

	//SE JUEGA EL TORNEO

	jugarTorneo(listaT.arrayT[a], listaT.arrayT[d], listaT.arrayT[b], listaT.arrayT[c]);

}

void jugarTorneo(tDatosTenista& tenista1, tDatosTenista& tenista2, tDatosTenista& tenista3, tDatosTenista& tenista4) {

	tTenista finalista1 = NADIE, finalista2 = NADIE, ganadorTorneo = NADIE;

	//SE ESTABLECE EL ORDEN DE LOS PARTIDOS

	cout << "* Primera semifinal: " << tenista1.iniciales << " vs " << tenista2.iniciales<<endl;
	cout << "* Segunda semifinal: " << tenista3.iniciales << " vs " << tenista4.iniciales<<endl<<endl;
	cout << "* COMIENZA EL TORNEO *" << endl << endl;
	system("pause");

	//PRIMERA SEMIFINAL

	cout << "* Primera semifinal: " << tenista1.iniciales << " vs " << tenista2.iniciales << endl;
	finalista1 = jugarPartido(tenista1, tenista2);

	//SEGUNA SEMIFINAL

	cout << "* Segunda semifinal: " << tenista3.iniciales << " vs " << tenista4.iniciales << endl;
	finalista2 = jugarPartido(tenista3, tenista4);

	//SE JUEGA LA FINAL Y SE DA EL GANADOR
	
	switch (finalista1) {

	case TENISTA1:

		switch (finalista2) {

		case TENISTA1:

			//FINAL DEL JUG 1 Y 3
			cout << "* Gran final: " << tenista1.iniciales << " vs " << tenista3.iniciales << endl;
			ganadorTorneo = jugarPartido(tenista1, tenista3);

			if (ganadorTorneo = TENISTA1) {

				cout << "*CAMPEON DEL TORNEO: " << tenista1.iniciales << endl;
			}
			else {

				cout << "*CAMPEON DEL TORNEO: " << tenista3.iniciales << endl;
			}

			break;

		case TENISTA2:

			//FINAL DEL JUG 1 Y 4
			cout << "* Gran final: " << tenista1.iniciales << " vs " << tenista4.iniciales << endl;
			ganadorTorneo = jugarPartido(tenista1, tenista4);

			if (ganadorTorneo = TENISTA1) {

				cout << "*CAMPEON DEL TORNEO: " << tenista1.iniciales << endl;
			}
			else {

				cout << "*CAMPEON DEL TORNEO: " << tenista4.iniciales << endl;
			}

			break;

		}

		break;

	case TENISTA2:

		switch (finalista2) {

		case TENISTA1:

			//FINAL DEL JUG 2 Y 3
			cout << "* Gran final: " << tenista2.iniciales << " vs " << tenista3.iniciales << endl;
			ganadorTorneo = jugarPartido(tenista2, tenista3);

			if (ganadorTorneo = TENISTA1) {

				cout << "*CAMPEON DEL TORNEO: " << tenista2.iniciales << endl;
			}
			else {

				cout << "*CAMPEON DEL TORNEO: " << tenista3.iniciales << endl;
			}

			break;

		case TENISTA2:

			//FINAL DEL JUG 2 Y 4
			cout << "* Gran final: " << tenista2.iniciales << " vs " << tenista4.iniciales << endl;
			ganadorTorneo = jugarPartido(tenista2, tenista4);

			if (ganadorTorneo = TENISTA1) {

				cout << "*CAMPEON DEL TORNEO: " << tenista2.iniciales << endl;
			}
			else {

				cout << "*CAMPEON DEL TORNEO: " << tenista4.iniciales << endl;
			}

			break;
		}

		break;
	}
}