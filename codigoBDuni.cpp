/*
 * Practica Evaluada - Programacion I
 * Aplicacion CRUD en C++ con base de datos MySQL usando ODBC
 *
 * Compilar en VS Code con:
 * g++ main.cpp -o universidad -lodbc32
 */

#include <iostream>
#include <windows.h>
#include <sqlext.h>

using namespace std;

// ============================================================
//  DATOS DE CONEXION
//  Cambia PASSWORD= por tu contrasena real de MySQL
//  Si no tienes contrasena dejalo como: PASSWORD=;
// ============================================================
const char* CONN_STR =
    "DRIVER={MySQL ODBC 9.7 Unicode Driver};"
    "SERVER=localhost;"
    "PORT=3306;"
    "DATABASE=universidad;"
    "USER=root;"
    "PASSWORD=m6666;";

// ============================================================
//  FUNCION: mostrarError
//  Muestra el mensaje de error cuando algo falla en la BD
// ============================================================
void mostrarError(SQLSMALLINT tipoHandle, SQLHANDLE handle) {
    SQLCHAR sqlState[6];
    SQLCHAR message[256];
    SQLINTEGER nativeError;
    SQLSMALLINT textLength;

    if (SQLGetDiagRec(tipoHandle, handle, 1, sqlState, &nativeError,
                      message, sizeof(message), &textLength) == SQL_SUCCESS) {
        cout << "  SQLSTATE: " << sqlState << endl;
        cout << "  Error: " << message << endl;
    }
}

// ============================================================
//  FUNCION: conectar
//  Abre la conexion con MySQL y la retorna
//  Si falla retorna NULL
// ============================================================
SQLHDBC conectar() {
    SQLHENV hEnv = NULL;
    SQLHDBC hDbc = NULL;
    SQLRETURN ret;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    SQLCHAR connStr[512];
    int i = 0;
    while (CONN_STR[i] != '\0') { connStr[i] = (SQLCHAR)CONN_STR[i]; i++; }
    connStr[i] = '\0';

    SQLCHAR outConnStr[1024];
    SQLSMALLINT outConnStrLen;

    ret = SQLDriverConnect(
        hDbc, NULL, connStr, SQL_NTS,
        outConnStr, sizeof(outConnStr), &outConnStrLen,
        SQL_DRIVER_NOPROMPT
    );

    if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
        cout << "Error al conectar a la base de datos." << endl;
        mostrarError(SQL_HANDLE_DBC, hDbc);
        return NULL;
    }

    cout << "Conexion exitosa a la base de datos.\n";
    return hDbc;
}


// ============================================================
//  ---- MODULO ALUMNOS ----
// ============================================================

// Lista alumnos con sus cursos y seccion usando JOIN
void listarAlumnos(SQLHDBC hDbc) {
    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT a.id, a.nombres, a.apellidos, a.carnet, "
        "IFNULL(c.nombre, 'Sin curso') AS curso, "
        "IFNULL(s.nombre, 'Sin seccion') AS seccion "
        "FROM alumnos a "
        "LEFT JOIN alumnos_cursos ac ON a.id = ac.alumno_id "
        "LEFT JOIN cursos c ON ac.curso_id = c.id "
        "LEFT JOIN alumnos_seccion ase ON a.id = ase.alumno_id "
        "LEFT JOIN secciones s ON ase.seccion_id = s.id";

    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
        cout << "Error al listar alumnos.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLINTEGER id;
    SQLCHAR nombres[51], apellidos[51], carnet[21], curso[101], seccion[51];

    cout << "\n--- Listado de Alumnos ---\n";
    cout << "ID | Nombres | Apellidos | Carnet | Curso | Seccion\n";
    cout << "------------------------------------------------------------\n";

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, nombres, sizeof(nombres), NULL);
        SQLGetData(hStmt, 3, SQL_C_CHAR, apellidos, sizeof(apellidos), NULL);
        SQLGetData(hStmt, 4, SQL_C_CHAR, carnet, sizeof(carnet), NULL);
        SQLGetData(hStmt, 5, SQL_C_CHAR, curso, sizeof(curso), NULL);
        SQLGetData(hStmt, 6, SQL_C_CHAR, seccion, sizeof(seccion), NULL);

        cout << id << " | " << nombres << " | " << apellidos
             << " | " << carnet << " | " << curso << " | " << seccion << endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void insertarAlumno(SQLHDBC hDbc) {
    char nombres[51], apellidos[51], carnet[21];

    cout << "\nNombres del alumno: ";
    cin.ignore();
    cin.getline(nombres, 51);
    cout << "Apellidos del alumno: ";
    cin.getline(apellidos, 51);
    cout << "Carnet del alumno: ";
    cin.getline(carnet, 21);

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "INSERT INTO alumnos (nombres, apellidos, carnet) VALUES (?, ?, ?)";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, nombres, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, apellidos, 0, NULL);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, carnet, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Alumno grabado correctamente.\n";
    else {
        cout << "Error al grabar alumno.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void actualizarAlumno(SQLHDBC hDbc) {
    int id;
    char nombres[51], apellidos[51], carnet[21];

    cout << "\nID del alumno a modificar: ";
    cin >> id;
    cout << "Nuevos nombres: ";
    cin.ignore();
    cin.getline(nombres, 51);
    cout << "Nuevos apellidos: ";
    cin.getline(apellidos, 51);
    cout << "Nuevo carnet: ";
    cin.getline(carnet, 21);

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "UPDATE alumnos SET nombres=?, apellidos=?, carnet=? WHERE id=?";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, nombres, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, apellidos, 0, NULL);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, carnet, 0, NULL);
    SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Alumno modificado correctamente.\n";
    else {
        cout << "Error al modificar alumno.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarAlumno(SQLHDBC hDbc) {
    int id;
    cout << "\nID del alumno a eliminar: ";
    cin >> id;

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "DELETE FROM alumnos WHERE id=?";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Alumno eliminado correctamente.\n";
    else {
        cout << "Error al eliminar alumno.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void menuAlumnos(SQLHDBC hDbc) {
    int opcion;
    do {
        cout << "\n===== MODULO ALUMNOS =====\n";
        cout << "1. Listar\n2. Grabar\n3. Modificar\n4. Eliminar\n5. Regresar\n";
        cout << "Opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1: listarAlumnos(hDbc);    break;
            case 2: insertarAlumno(hDbc);   break;
            case 3: actualizarAlumno(hDbc); break;
            case 4: eliminarAlumno(hDbc);   break;
            case 5: cout << "Regresando...\n"; break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 5);
}


// ============================================================
//  ---- MODULO CURSOS ----
// ============================================================

void listarCursos(SQLHDBC hDbc) {
    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT id, nombre, codigo FROM cursos";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
        cout << "Error al listar cursos.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLINTEGER id;
    SQLCHAR nombre[101], codigo[21];

    cout << "\n--- Listado de Cursos ---\n";
    cout << "ID | Nombre | Codigo\n";
    cout << "--------------------------------\n";

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
        SQLGetData(hStmt, 3, SQL_C_CHAR, codigo, sizeof(codigo), NULL);
        cout << id << " | " << nombre << " | " << codigo << endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void insertarCurso(SQLHDBC hDbc) {
    char nombre[101], codigo[21];

    cout << "\nNombre del curso: ";
    cin.ignore();
    cin.getline(nombre, 101);
    cout << "Codigo del curso: ";
    cin.getline(codigo, 21);

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "INSERT INTO cursos (nombre, codigo) VALUES (?, ?)";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, nombre, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, codigo, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Curso grabado correctamente.\n";
    else {
        cout << "Error al grabar curso.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void actualizarCurso(SQLHDBC hDbc) {
    int id;
    char nombre[101], codigo[21];

    cout << "\nID del curso a modificar: ";
    cin >> id;
    cout << "Nuevo nombre: ";
    cin.ignore();
    cin.getline(nombre, 101);
    cout << "Nuevo codigo: ";
    cin.getline(codigo, 21);

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "UPDATE cursos SET nombre=?, codigo=? WHERE id=?";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, nombre, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, codigo, 0, NULL);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Curso modificado correctamente.\n";
    else {
        cout << "Error al modificar curso.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarCurso(SQLHDBC hDbc) {
    int id;
    cout << "\nID del curso a eliminar: ";
    cin >> id;

    // Verificar si el curso tiene alumnos asignados antes de eliminar
    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR queryCheck[] = "SELECT COUNT(*) FROM alumnos_cursos WHERE curso_id=?";
    SQLPrepare(hStmt, queryCheck, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);
    SQLExecute(hStmt);

    SQLINTEGER cantidad = 0;
    if (SQLFetch(hStmt) == SQL_SUCCESS)
        SQLGetData(hStmt, 1, SQL_C_LONG, &cantidad, 0, NULL);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    if (cantidad > 0) {
        cout << "No se puede eliminar: el curso tiene " << cantidad << " alumno(s) asignado(s).\n";
        return;
    }

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLCHAR query[] = "DELETE FROM cursos WHERE id=?";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Curso eliminado correctamente.\n";
    else {
        cout << "Error al eliminar curso.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void menuCursos(SQLHDBC hDbc) {
    int opcion;
    do {
        cout << "\n===== MODULO CURSOS =====\n";
        cout << "1. Listar\n2. Grabar\n3. Modificar\n4. Eliminar\n5. Regresar\n";
        cout << "Opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1: listarCursos(hDbc);    break;
            case 2: insertarCurso(hDbc);   break;
            case 3: actualizarCurso(hDbc); break;
            case 4: eliminarCurso(hDbc);   break;
            case 5: cout << "Regresando...\n"; break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 5);
}


// ============================================================
//  ---- MODULO SECCIONES ----
// ============================================================

void listarSecciones(SQLHDBC hDbc) {
    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT id, nombre, jornada FROM secciones";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
        cout << "Error al listar secciones.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLINTEGER id;
    SQLCHAR nombre[51], jornada[31];

    cout << "\n--- Listado de Secciones ---\n";
    cout << "ID | Nombre | Jornada\n";
    cout << "--------------------------------\n";

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
        SQLGetData(hStmt, 3, SQL_C_CHAR, jornada, sizeof(jornada), NULL);
        cout << id << " | " << nombre << " | " << jornada << endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void insertarSeccion(SQLHDBC hDbc) {
    char nombre[51], jornada[31];

    cout << "\nNombre de la seccion: ";
    cin.ignore();
    cin.getline(nombre, 51);
    cout << "Jornada (Matutina/Vespertina/Nocturna): ";
    cin.getline(jornada, 31);

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "INSERT INTO secciones (nombre, jornada) VALUES (?, ?)";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, nombre, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30, 0, jornada, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Seccion grabada correctamente.\n";
    else {
        cout << "Error al grabar seccion.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void actualizarSeccion(SQLHDBC hDbc) {
    int id;
    char nombre[51], jornada[31];

    cout << "\nID de la seccion a modificar: ";
    cin >> id;
    cout << "Nuevo nombre: ";
    cin.ignore();
    cin.getline(nombre, 51);
    cout << "Nueva jornada: ";
    cin.getline(jornada, 31);

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "UPDATE secciones SET nombre=?, jornada=? WHERE id=?";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, nombre, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30, 0, jornada, 0, NULL);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Seccion modificada correctamente.\n";
    else {
        cout << "Error al modificar seccion.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarSeccion(SQLHDBC hDbc) {
    int id;
    cout << "\nID de la seccion a eliminar: ";
    cin >> id;

    // Verificar si la seccion tiene alumnos asignados antes de eliminar
    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR queryCheck[] = "SELECT COUNT(*) FROM alumnos_seccion WHERE seccion_id=?";
    SQLPrepare(hStmt, queryCheck, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);
    SQLExecute(hStmt);

    SQLINTEGER cantidad = 0;
    if (SQLFetch(hStmt) == SQL_SUCCESS)
        SQLGetData(hStmt, 1, SQL_C_LONG, &cantidad, 0, NULL);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    if (cantidad > 0) {
        cout << "No se puede eliminar: la seccion tiene " << cantidad << " alumno(s) asignado(s).\n";
        return;
    }

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLCHAR query[] = "DELETE FROM secciones WHERE id=?";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Seccion eliminada correctamente.\n";
    else {
        cout << "Error al eliminar seccion.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void menuSecciones(SQLHDBC hDbc) {
    int opcion;
    do {
        cout << "\n===== MODULO SECCIONES =====\n";
        cout << "1. Listar\n2. Grabar\n3. Modificar\n4. Eliminar\n5. Regresar\n";
        cout << "Opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1: listarSecciones(hDbc);   break;
            case 2: insertarSeccion(hDbc);   break;
            case 3: actualizarSeccion(hDbc); break;
            case 4: eliminarSeccion(hDbc);   break;
            case 5: cout << "Regresando...\n"; break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 5);
}


// ============================================================
//  ---- MODULO ALUMNOS - CURSOS ----
// ============================================================

void listarAlumnosCursos(SQLHDBC hDbc) {
    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT ac.id, a.nombres, a.apellidos, c.nombre AS curso "
        "FROM alumnos_cursos ac "
        "JOIN alumnos a ON ac.alumno_id = a.id "
        "JOIN cursos c ON ac.curso_id = c.id";

    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
        cout << "Error al listar asignaciones.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLINTEGER id;
    SQLCHAR nombres[51], apellidos[51], curso[101];

    cout << "\n--- Asignaciones Alumnos-Cursos ---\n";
    cout << "ID | Alumno | Curso\n";
    cout << "--------------------------------------------\n";

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, nombres, sizeof(nombres), NULL);
        SQLGetData(hStmt, 3, SQL_C_CHAR, apellidos, sizeof(apellidos), NULL);
        SQLGetData(hStmt, 4, SQL_C_CHAR, curso, sizeof(curso), NULL);
        cout << id << " | " << nombres << " " << apellidos << " | " << curso << endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void asignarCursoAlumno(SQLHDBC hDbc) {
    int alumnoId, cursoId;

    listarAlumnos(hDbc);
    cout << "\nID del alumno: ";
    cin >> alumnoId;

    listarCursos(hDbc);
    cout << "\nID del curso a asignar: ";
    cin >> cursoId;

    // Verificar que no se asigne el mismo curso dos veces
    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR queryCheck[] = "SELECT COUNT(*) FROM alumnos_cursos WHERE alumno_id=? AND curso_id=?";
    SQLPrepare(hStmt, queryCheck, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &alumnoId, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &cursoId, 0, NULL);
    SQLExecute(hStmt);

    SQLINTEGER existe = 0;
    if (SQLFetch(hStmt) == SQL_SUCCESS)
        SQLGetData(hStmt, 1, SQL_C_LONG, &existe, 0, NULL);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    if (existe > 0) {
        cout << "El alumno ya tiene ese curso asignado.\n";
        return;
    }

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLCHAR query[] = "INSERT INTO alumnos_cursos (alumno_id, curso_id) VALUES (?, ?)";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &alumnoId, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &cursoId, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Curso asignado correctamente.\n";
    else {
        cout << "Error al asignar curso.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarAsignacionCurso(SQLHDBC hDbc) {
    int id;
    listarAlumnosCursos(hDbc);
    cout << "\nID de la asignacion a eliminar: ";
    cin >> id;

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "DELETE FROM alumnos_cursos WHERE id=?";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Asignacion eliminada correctamente.\n";
    else {
        cout << "Error al eliminar asignacion.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void menuAlumnosCursos(SQLHDBC hDbc) {
    int opcion;
    do {
        cout << "\n===== MODULO ALUMNOS - CURSOS =====\n";
        cout << "1. Listar\n2. Asignar curso a alumno\n3. Eliminar asignacion\n4. Regresar\n";
        cout << "Opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1: listarAlumnosCursos(hDbc);     break;
            case 2: asignarCursoAlumno(hDbc);      break;
            case 3: eliminarAsignacionCurso(hDbc); break;
            case 4: cout << "Regresando...\n"; break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 4);
}


// ============================================================
//  ---- MODULO ALUMNOS - SECCION ----
// ============================================================

void listarAlumnosSeccion(SQLHDBC hDbc) {
    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT ase.id, a.nombres, a.apellidos, s.nombre AS seccion "
        "FROM alumnos_seccion ase "
        "JOIN alumnos a ON ase.alumno_id = a.id "
        "JOIN secciones s ON ase.seccion_id = s.id";

    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
        cout << "Error al listar asignaciones de seccion.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLINTEGER id;
    SQLCHAR nombres[51], apellidos[51], seccion[51];

    cout << "\n--- Asignaciones Alumnos-Seccion ---\n";
    cout << "ID | Alumno | Seccion\n";
    cout << "--------------------------------------------\n";

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, nombres, sizeof(nombres), NULL);
        SQLGetData(hStmt, 3, SQL_C_CHAR, apellidos, sizeof(apellidos), NULL);
        SQLGetData(hStmt, 4, SQL_C_CHAR, seccion, sizeof(seccion), NULL);
        cout << id << " | " << nombres << " " << apellidos << " | " << seccion << endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void asignarSeccionAlumno(SQLHDBC hDbc) {
    int alumnoId, seccionId;

    listarAlumnos(hDbc);
    cout << "\nID del alumno: ";
    cin >> alumnoId;

    listarSecciones(hDbc);
    cout << "\nID de la seccion: ";
    cin >> seccionId;

    // Verificar que el alumno no tenga ya una seccion asignada
    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR queryCheck[] = "SELECT COUNT(*) FROM alumnos_seccion WHERE alumno_id=?";
    SQLPrepare(hStmt, queryCheck, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &alumnoId, 0, NULL);
    SQLExecute(hStmt);

    SQLINTEGER existe = 0;
    if (SQLFetch(hStmt) == SQL_SUCCESS)
        SQLGetData(hStmt, 1, SQL_C_LONG, &existe, 0, NULL);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    if (existe > 0) {
        cout << "El alumno ya tiene una seccion asignada. Use modificar para cambiarla.\n";
        return;
    }

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLCHAR query[] = "INSERT INTO alumnos_seccion (alumno_id, seccion_id) VALUES (?, ?)";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &alumnoId, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &seccionId, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Seccion asignada correctamente.\n";
    else {
        cout << "Error al asignar seccion.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void modificarSeccionAlumno(SQLHDBC hDbc) {
    int id, nuevaSeccionId;

    listarAlumnosSeccion(hDbc);
    cout << "\nID de la asignacion a modificar: ";
    cin >> id;

    listarSecciones(hDbc);
    cout << "\nID de la nueva seccion: ";
    cin >> nuevaSeccionId;

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "UPDATE alumnos_seccion SET seccion_id=? WHERE id=?";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nuevaSeccionId, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Seccion modificada correctamente.\n";
    else {
        cout << "Error al modificar seccion.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarAsignacionSeccion(SQLHDBC hDbc) {
    int id;
    listarAlumnosSeccion(hDbc);
    cout << "\nID de la asignacion a eliminar: ";
    cin >> id;

    SQLHSTMT hStmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "DELETE FROM alumnos_seccion WHERE id=?";
    SQLPrepare(hStmt, query, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        cout << "Asignacion eliminada correctamente.\n";
    else {
        cout << "Error al eliminar asignacion.\n";
        mostrarError(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void menuAlumnosSeccion(SQLHDBC hDbc) {
    int opcion;
    do {
        cout << "\n===== MODULO ALUMNOS - SECCION =====\n";
        cout << "1. Listar\n2. Asignar seccion a alumno\n3. Modificar seccion\n4. Eliminar asignacion\n5. Regresar\n";
        cout << "Opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1: listarAlumnosSeccion(hDbc);      break;
            case 2: asignarSeccionAlumno(hDbc);      break;
            case 3: modificarSeccionAlumno(hDbc);    break;
            case 4: eliminarAsignacionSeccion(hDbc); break;
            case 5: cout << "Regresando...\n"; break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 5);
}


// ============================================================
//  MAIN - Punto de inicio del programa
// ============================================================
int main() {
    SQLHDBC hDbc = conectar();

    if (hDbc == NULL) {
        cout << "No se pudo conectar. Verifica los datos de conexion.\n";
        return 1;
    }

    int opcion;

    do {
        cout << "\n===== SISTEMA UNIVERSIDAD =====\n";
        cout << "1. Alumnos\n";
        cout << "2. Cursos\n";
        cout << "3. Secciones\n";
        cout << "4. Alumnos - Cursos\n";
        cout << "5. Alumnos - Seccion\n";
        cout << "6. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: menuAlumnos(hDbc);        break;
            case 2: menuCursos(hDbc);         break;
            case 3: menuSecciones(hDbc);      break;
            case 4: menuAlumnosCursos(hDbc);  break;
            case 5: menuAlumnosSeccion(hDbc); break;
            case 6: cout << "Cerrando programa...\n"; break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 6);

    // Cerrar la conexion antes de salir
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);

    cout << "Conexion cerrada. Hasta luego.\n";
    return 0;
}