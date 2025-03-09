# Rick and Morty Batch App

![Rick and Morty](https://cdn.hobbyconsolas.com/sites/navi.axelspringer.es/public/media/image/2016/10/rick-morty-critica.jpg)

## Language / Idioma

- [English](#english-version)
- [Español](#versión-en-español)

## English Version

### Project Description

**Rick and Morty Batch App** is a console application designed to interact with the Rick and Morty API, manage character, episode, and location data, and configure logging and database options. The application offers a colorful console user interface with intuitive navigation.

### Features

- **Interaction with the Rick and Morty API**: Load and display character, episode, and location data.
- **Database Management**: Filter and search stored data in the database.
- **Custom Configuration**: Adjust API parameters, database, and logging level.
- **Colorful Menus**: Use ANSI colors to enhance the visual experience in the console.
- **Data Loading**: Data can be loaded in two ways:
  - **Via JSON**: Use a JSON file to load the initial configuration. The JSON format should be:

    ```json 
    {
        "database": {
            "connection_string": "dbname=rickandmorty user=user password=password host=localhost port=5432"
        },
        "api": {
            "base_url": "https://rickandmortyapi.com/api",
            "update_time": 100
        }
    }
    ```

    The `update_time` and `dbname` values are optional. If not provided, the default values of 24h and `rickandmorty` are used.
    
      **Note**: Using JSON for configuration can be less secure due to the inclusion of plain text passwords. It is recommended to set default values in the JSON and modify them through the user interface.
  - **Via User Interface**: Parameters configured in the JSON can be modified later through the user interface.
- **Automatic Data Update**: The application is designed to request information from the API periodically. This update interval is configurable:
  - **Thread Control**: Uses mutex to safely manage thread control.
  - **Update Time Configuration**: The "update_time" can be set in seconds. If not specified, the default is every 24 hours.
  - **Manual Requests**: In addition to automatic updates, the application allows manual data requests.

### Main Menus

#### Main Menu

- **1. Load data from API**: Load data from the Rick and Morty API.
- **2. Filter and search data from database**: Filter and search data in the database.
- **3. Show Configuration Menu**: Show the configuration menu.
- **4. Exit**: Exit the application.

#### Configuration Menu

- **1. Load configuration from file**: Load configuration from a file.
- **2. Configure API URL**: Configure the API URL.
- **3. Configure Database Parameters**: Configure database parameters.
- **4. Configure Time Batch Reconnection**: Configure reconnection time.
- **5. Logger**: Access the logger configuration menu.
- **6. Continue to main menu**: Return to the main menu.

#### Logger Menu

- **1. Show File Path Log**: Show the log file location.
- **2. Set log level**: Set the log level.
- **3. Set log file path**: Set the log file path.
- **4. Enable/Disable logger**: Enable or disable the logger.
- **5. Return to configuration menu**: Return to the configuration menu.

### System Requirements

- **C++ Compiler**: Compatible with C++20.
  - Compiled with: MSVC 19.29.30157.0
- **CMake**: For project setup, version used "cmake version 4.0.0-rc3"
- **Required Libraries**:
  - [libcurl](https://curl.se/libcurl/) for HTTP requests.
  - [jsoncpp](https://github.com/open-source-parsers/jsoncpp) for JSON data handling.
  - [spdlog](https://github.com/gabime/spdlog) for logging.
  - [pqxx](https://github.com/jtv/libpqxx) for PostgreSQL interaction.
  - **PostgreSQL**: Essential for database management.

### PostgreSQL Setup

1. **Download and Install PostgreSQL**:
   - Visit the [official PostgreSQL site](https://www.postgresql.org/download/) and download the version compatible with your OS.
   - Follow the installer instructions to complete the installation. Remember the password you set for the `postgres` user.

2. **Database Configuration**:
   - No need to manually create a database named `rickandmorty`, as the application can create it automatically if it has the necessary permissions.
   - Ensure the PostgreSQL service is running before starting the application.
   - If the application encounters an error and cannot automatically create the tables, you will need to create them manually.

### Compilation Instructions

1. **Clone the Repository**:

```bash git clone https://github.com/your_username/RickAndMortyBatch.git cd RickAndMortyBatch```
2. **Configure the Project**:
   Ensure CMake is installed and run:

```bash mkdir build cd build cmake ..```
3. **Compile the Project**:

```bash cmake --build .```
4. **Run the Application**:

bash RickAndMortyBatch.exe
### Usage

- **Menu Navigation**: Use number keys to select options in the menus.
- **Console Colors**: The application uses ANSI colors to enhance visualization. Ensure your terminal supports these colors.

## Versión en Español

### Descripción del Proyecto

**Rick and Morty Batch App** es una aplicación de consola diseñada para interactuar con la API de Rick and Morty, gestionar datos de personajes, episodios y ubicaciones, y configurar opciones de registro y base de datos. La aplicación ofrece una interfaz de usuario en la consola con menús coloridos y una navegación intuitiva.

### Características

- **Interacción con la API de Rick and Morty**: Carga y muestra datos de personajes, episodios y ubicaciones.
- **Gestión de Base de Datos**: Filtra y busca datos almacenados en la base de datos.
- **Configuración Personalizada**: Ajusta parámetros de API, base de datos y nivel de registro.
- **Menús Coloridos**: Usa colores ANSI para mejorar la experiencia visual en la consola.
- **Carga de Datos**: Los datos pueden ser cargados de dos maneras:
  - **Por JSON**: Utiliza un archivo JSON para cargar la configuración inicial. El formato del JSON debe ser:

    ```json
    {
        "database": {
            "connection_string": "dbname=rickandmorty user=user password=password host=localhost port=5432"
        },
        "api": {
            "base_url": "https://rickandmortyapi.com/api",
            "update_time": 100
        }
    }
    ```
    
    Los valores `update_time` y `dbname` son opcionales, en caso de no ponerlos se pondrán los valores por defecto de la App que son 24h y `rickandmorty`.
    
      **Nota**: Usar JSON para la configuración puede ser menos seguro debido a la inclusión de contraseñas en texto plano. Es recomendable establecer valores predeterminados en el JSON y luego modificarlos a través de la interfaz de usuario.
  - **Por Interfaz de Usuario**: Los parámetros configurados en el JSON pueden ser modificados posteriormente a través de la interfaz de usuario.
- **Actualización Automática de Datos**: La aplicación está diseñada para solicitar información de la API cada cierto tiempo. Este intervalo de actualización es configurable:
  - **Control de Hilos**: Utiliza mutex para manejar el control de los hilos de manera segura.
  - **Configuración de Tiempo de Actualización**: El tiempo de actualización "update_time" se puede configurar en segundos. Si no se especifica, el valor predeterminado es cada 24 horas.
  - **Solicitudes Manuales**: Además de la actualización automática, la aplicación permite realizar solicitudes manuales de datos.

### Menús Principales

#### Menú Principal

- **1. Load data from API**: Carga datos desde la API de Rick and Morty.
- **2. Filter and search data from database**: Filtra y busca datos en la base de datos.
- **3. Show Configuration Menu**: Muestra el menú de configuración.
- **4. Exit**: Salir de la aplicación.

#### Menú de Configuración

- **1. Load configuration from file**: Carga la configuración desde un archivo.
- **2. Configure API URL**: Configura la URL de la API.
- **3. Configure Database Parameters**: Configura los parámetros de la base de datos.
- **4. Configure Time Batch Reconnection**: Configura el tiempo de reconexión.
- **5. Logger**: Accede al menú de configuración del logger.
- **6. Continue to main menu**: Regresa al menú principal.

#### Menú de Logger

- **1. Show File Path Log**: Muestra la ubicación del archivo de log.
- **2. Set log level**: Configura el nivel de log.
- **3. Set log file path**: Configura la ruta del archivo de log.
- **4. Enable/Disable logger**: Habilita o deshabilita el logger.
- **5. Return to configuration menu**: Regresa al menú de configuración.

### Requisitos del Sistema

- **C++ Compiler**: Compatible con C++20.
  - Compilado con: MSVC 19.29.30157.0
- **CMake**: Para la configuración del proyecto, versión utilizada "cmake version 4.0.0-rc3"
- **Bibliotecas necesarias**:
  - [libcurl](https://curl.se/libcurl/) para realizar solicitudes HTTP.
  - [jsoncpp](https://github.com/open-source-parsers/jsoncpp) para el manejo de datos JSON.
  - [spdlog](https://github.com/gabime/spdlog) para el registro.
  - [pqxx](https://github.com/jtv/libpqxx) para la interacción con PostgreSQL.
  - **PostgreSQL**: Requisito indispensable para la gestión de la base de datos.

### Configuración de PostgreSQL

1. **Descargar e Instalar PostgreSQL**:
   - Visita el [sitio oficial de PostgreSQL](https://www.postgresql.org/download/) y descarga la versión compatible con tu sistema operativo.
   - Sigue las instrucciones del instalador para completar la instalación. Asegúrate de recordar la contraseña que establezcas para el usuario `postgres`.

2. **Configurar la Base de Datos**:
   - No es necesario crear manualmente una base de datos llamada `rickandmorty`, ya que la aplicación puede crearla automáticamente si tiene los permisos necesarios.
   - Asegúrate de que el servicio de PostgreSQL esté en ejecución antes de iniciar la aplicación.
   - En caso de que la aplicación genere un error y no pueda crear las tablas automáticamente, deberás crearlas manualmente.

### Instrucciones de Compilación

1. **Clona el Repositorio**:

```bash git clone https://github.com/tu_usuario/RickAndMortyBatch.git cd RickAndMortyBatch```
2. **Configura el Proyecto**:
   Asegúrate de tener CMake instalado y ejecuta:

```bash mkdir build cd build cmake ..```
3. **Compila el Proyecto**:

bash cmake --build .
4. **Ejecuta la Aplicación**:

bash RickAndMortyBatch.exe
### Uso

- **Navegación por Menús**: Usa las teclas numéricas para seleccionar opciones en los menús.
- **Colores en Consola**: La aplicación utiliza colores ANSI para mejorar la visualización. Asegúrate de que tu terminal soporte estos colores.