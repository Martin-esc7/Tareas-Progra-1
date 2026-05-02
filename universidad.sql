CREATE DATABASE IF NOT EXISTS universidad;
USE universidad;

CREATE TABLE alumnos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombres VARCHAR(100),
    apellidos VARCHAR(100),
    carnet VARCHAR(20)
);

CREATE TABLE cursos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100),
    codigo VARCHAR(20)
);

CREATE TABLE secciones (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100),
    jornada VARCHAR(50)
);

CREATE TABLE alumnos_cursos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    alumno_id INT,
    curso_id INT,
    FOREIGN KEY (alumno_id) REFERENCES alumnos(id),
    FOREIGN KEY (curso_id) REFERENCES cursos(id)
);

CREATE TABLE alumnos_seccion (
    id INT AUTO_INCREMENT PRIMARY KEY,
    alumno_id INT,
    seccion_id INT,
    FOREIGN KEY (alumno_id) REFERENCES alumnos(id),
    FOREIGN KEY (seccion_id) REFERENCES secciones(id)
);