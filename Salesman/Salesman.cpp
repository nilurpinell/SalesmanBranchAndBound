
// Salesman.cpp : define los comportamientos de las clases para la aplicaci�n.
//

#include "stdafx.h"
#include "Salesman.h"
#include "SalesmanDlg.h"
#include <io.h>
#include <fcntl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSalesmanApp

BEGIN_MESSAGE_MAP(CSalesmanApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Construcci�n de CSalesmanApp

CSalesmanApp::CSalesmanApp()
{
	// TODO: agregar aqu� el c�digo de construcci�n,
	// Colocar toda la inicializaci�n importante en InitInstance
}


// =============================================================================
// IDENTIFICACION DE LOS ALUMNOS ===============================================
// =============================================================================

CString NombreAlumno1 = "Agusti";
CString ApellidosAlumno1 = "Sobejano Ventayol";
CString NIAAlumno1 = "1425109"; // NIA alumno1

// No rellenar en caso de grupo de un alumno
CString NombreAlumno2 = "Nil";
CString ApellidosAlumno2 = "Urpinell Jimenez";
CString NIAAlumno2 = "1428176"; // NIA alumno2

char* NIAS[] = {
	"1335167","1428136","1397750","1390855","1424739","1424670","1423739",
	"1423177","1427371","1423725","1448752","1423210","1391968","1424454",
	"1338429","1425988","1424818","1421980","1368533","1365501","1423802",
	"1175542","1264719","1424801","1423710","1390452","1424116","1391795",
	"1395085","1391630","1424310","1423234","1425968","1307828","1395062",
	"1426532","1391627","1366098","1425082","1325835","1425067","1424350",
	"1424035","1391544","1391634","1426771","1424698","1358835","1362219",
	"1425785","1424114","1424408","1362389","1428260","1391808","1425084",
	"1366706","1391632","1425109","1424032","1430970","1430896","1428176",
	"1390536","1429086","1423588","1195650","1424671","1306347"
};

bool CheckNIA(CString nia)
{
	for (const char *pNIA : NIAS) {
		if (nia.Compare(pNIA) == 0) return true;
	}
	return false;
}

// El �nico objeto CSalesmanApp

CSalesmanApp theApp;

// Clock =======================================================================

double Clock()
{
	LARGE_INTEGER cnt;
	LARGE_INTEGER fre;

	if (QueryPerformanceFrequency(&fre) && QueryPerformanceCounter(&cnt)) {
		return (double)cnt.QuadPart / (double)fre.QuadPart;
	}
	else return (double)clock() / (double)CLOCKS_PER_SEC;
}

// ReadCommandArguments ========================================================

void ReadCommandArguments(int &nArgs, CString args[])
{
	nArgs = 0;
	char* pCommand = GetCommandLineA();
	//cout << pCommand << endl;
	char *pArg = pCommand;
	// Saltar el nombre del ejecutable
	while (*pArg <= ' ') if (*pArg) ++pArg; else return;
	if (*pArg == '\"') {
		// Leer hasta "
		++pArg;
		while (*pArg != '\"') {
			if (*pArg) ++pArg;
			else return;
		}
		++pArg;
	}
	else {
		while (*pArg > ' ') {
			if (*pArg) ++pArg;
			else return;
		}
	}
	// Saltar espacios
	while (*pArg == ' ') ++pArg;
	if (*pArg == '\0') return;
	//cout << "Linea de comandos " << (int) *pArg << endl;
	//AttachConsole(ATTACH_PARENT_PROCESS);
	//ofstream *pout = new ofstream("CONOUT$", ofstream::out | ofstream::app);
	//cout.rdbuf(pout->rdbuf());

	while (*pArg != '\0') {
		// Saltar espacios
		while (*pArg == ' ') ++pArg;
		if (*pArg == '\0') break;
		else if (*pArg == '\"') {
			++pArg;
			for (;;) {
				if (*pArg == '\0') break;
				else if (*pArg == '\"') break;
				args[nArgs].AppendChar(*pArg);
				++pArg;
			}
		}
		else {
			while (*pArg > ' ') {
				args[nArgs].AppendChar(*pArg);
				++pArg;
			}
		}
		++nArgs;
		if (nArgs >= 10) {
			return;
		}
	}
}

// EjecutarLineaDeComandos =====================================================

bool EjecutarLineaDeComandos(int nArgs, CString args[])
{
	if (nArgs < 3) {
		cout << "N�mero de argumentos erroneo" << endl;
		return true;
	}
	cout << "PARAMETROS:" << endl;
	if (args[1].CompareNoCase("dijkstra") == 0) {
		// dijkstra grafo distancias
		if (nArgs != 4) {
			cout << "N�mero de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Distancias.: " << args[3] << endl;
		try {
			CGraph graph;
			graph.Read(args[2]);
			double t0 = Clock();
			graph.Dijkstra(&graph.m_Vertices.front());
			double t1 = Clock();
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
			graph.WriteDistances(args[3]);
		}
		catch (exception &ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("dijkstraqueue") == 0) {
		// dijkstraqueue grafo distancias
		if (nArgs != 4) {
			cout << "N�mero de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Distancias.: " << args[3] << endl;
		try {
			CGraph graph;
			graph.Read(args[2]);
			double t0 = Clock();
			graph.DijkstraQueue(&graph.m_Vertices.front());
			double t1 = Clock();
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
			graph.WriteDistances(args[3]);
		}
		catch (exception &ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("greedy") == 0) {
		if (nArgs != 5) {
			cout << "N�mero de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph;
			graph.Read(args[2]);
			CVisits visits(&graph);
			visits.Read(args[3]);
			double t0 = Clock();
			CTrack track=graph.SalesmanTrackGreedy(visits);
			double t1 = Clock();
			track.Write(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception &ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("backtracking") == 0) {
		if (nArgs != 5) {
			cout << "N�mero de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph;
			graph.Read(args[2]);
			CVisits visits(&graph);
			visits.Read(args[3]);
			double t0 = Clock();
			CTrack track = graph.SalesmanTrackBacktracking(visits);
			double t1 = Clock();
			track.Write(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception &ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("backtrackinggreedy") == 0) {
		if (nArgs != 5) {
			cout << "N�mero de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph;
			graph.Read(args[2]);
			CVisits visits(&graph);
			visits.Read(args[3]);
			double t0 = Clock();
			CTrack track = graph.SalesmanTrackBacktrackingGreedy(visits);
			double t1 = Clock();
			track.Write(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception &ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("branchandbound1") == 0) {
		if (nArgs != 5) {
			cout << "N�mero de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph;
			graph.Read(args[2]);
			CVisits visits(&graph);
			visits.Read(args[3]);
			double t0 = Clock();
			CTrack track = graph.SalesmanTrackBranchAndBound1(visits);
			double t1 = Clock();
			track.Write(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception &ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("branchandbound2") == 0) {
		if (nArgs != 5) {
			cout << "N�mero de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph;
			graph.Read(args[2]);
			CVisits visits(&graph);
			visits.Read(args[3]);
			double t0 = Clock();
			CTrack track = graph.SalesmanTrackBranchAndBound2(visits);
			double t1 = Clock();
			track.Write(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception &ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("branchandbound3") == 0) {
		if (nArgs != 5) {
			cout << "N�mero de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph;
			graph.Read(args[2]);
			CVisits visits(&graph);
			visits.Read(args[3]);
			double t0 = Clock();
			CTrack track = graph.SalesmanTrackBranchAndBound3(visits);
			double t1 = Clock();
			track.Write(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << setprecision(9) << t1 - t0 << endl;
		}
		catch (exception &ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else {
		cout << "Error en el nombre del algoritmo" << endl;
	}
	return true;
}

// =============================================================================
// Inicializaci�n de CSalesmanApp ==============================================
// =============================================================================

BOOL CSalesmanApp::InitInstance()
{
	int nArgs;
	CString args[10];
	ReadCommandArguments(nArgs,args);
	CString outputName = "Output.txt";
	if (nArgs > 1) outputName = args[0];
	bool lineaDeComandos = false;
	if (AttachConsole(ATTACH_PARENT_PROCESS)) {
		ofstream *pout = new ofstream(outputName);
		cout.rdbuf(pout->rdbuf());
		lineaDeComandos = true;
	}
	else {
		AllocConsole();
		ofstream *pout = new ofstream("CONOUT$");
		cout.rdbuf(pout->rdbuf());
	}
	bool error = false;
	cout << "Grupo:" << endl;
	cout << "Alumno1:" << endl;
	cout << NombreAlumno1 << endl;
	cout << ApellidosAlumno1 << endl;
	cout << NIAAlumno1;
	if (!CheckNIA(NIAAlumno1)) {
		cout << " NIA ERRONEO!!!";
		error = true;
	}
	cout << endl;
	if (NIAAlumno2 != "") {
		cout << "Alumno2:" << endl;
		cout << NombreAlumno2 << endl;
		cout << ApellidosAlumno2 << endl;
		cout << NIAAlumno2;
		if (!CheckNIA(NIAAlumno1)) {
			cout << " NIA ERRONEO!!!";
			error = true;
		}
		cout << endl;
	}
	if (error) {
		if (!lineaDeComandos) AfxMessageBox("ERROR EN LA IDENTIFICACION DE LOS ALUMNOS", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Funcionamiento de la aplicaci�n s�lo en modo consola si tiene par�metros
	if (nArgs>0 && EjecutarLineaDeComandos(nArgs,args)) return FALSE;

	// Funcionamiento con dialogo =================================================
	// Windows XP requiere InitCommonControlsEx() si un manifiesto de
	// aplicaci�n especifica el uso de ComCtl32.dll versi�n 6 o posterior para habilitar
	// estilos visuales.  De lo contrario, se generar� un error al crear ventanas.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Establecer para incluir todas las clases de control comunes que desee utilizar
	// en la aplicaci�n.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Crear el administrador de shell, en caso de que el cuadro de di�logo contenga
	// alguna vista de �rbol de shell o controles de vista de lista de shell.
	CShellManager *pShellManager = new CShellManager;

	// Activar el administrador visual "nativo de Windows" para habilitar temas en controles MFC
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Inicializaci�n est�ndar
	// Si no utiliza estas funcionalidades y desea reducir el tama�o
	// del archivo ejecutable final, debe quitar
	// las rutinas de inicializaci�n espec�ficas que no necesite
	// Cambie la clave del Registro en la que se almacena la configuraci�n
	// TODO: debe modificar esta cadena para que contenga informaci�n correcta
	// como el nombre de su compa��a u organizaci�n
	SetRegistryKey(_T("Aplicaciones generadas con el Asistente para aplicaciones local"));

	// Abrir la consola --------------------------------------------------------



	CSalesmanDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: insertar aqu� el c�digo para controlar
		//  cu�ndo se descarta el cuadro de di�logo con Aceptar
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: insertar aqu� el c�digo para controlar
		//  cu�ndo se descarta el cuadro de di�logo con Cancelar
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Advertencia: la aplicaci�n est� finalizando porque hubo un error al crear el cuadro de di�logo.\n");
		TRACE(traceAppMsg, 0, "Advertencia: si usa controles MFC en el cuadro de di�logo, no puede usar #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Eliminar el administrador de shell creado anteriormente.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Dado que el cuadro de di�logo se ha cerrado, devolver FALSE para salir
	//  de la aplicaci�n en vez de iniciar el suministro de mensajes de dicha aplicaci�n.
	return FALSE;
}

