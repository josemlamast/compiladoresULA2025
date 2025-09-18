# Sistema de Entornos (Environment System)

## Estructura del Sistema

### 1. **Environment Global (Estático)**
```cpp
Environment global_env;  // Declarado en parser.bison y main.cpp
```
- **Ubicación**: Variable global estática
- **Propósito**: Almacena las definiciones de funciones
- **Acceso**: A través de `extern Environment global_env;`
- **Vida útil**: Durante toda la ejecución del programa

### 2. **Entornos Dinámicos (Locales)**

#### A. **CallExpression** - Entorno de Función
```cpp
std::shared_ptr<Expression> CallExpression::eval(Environment& env) const {
    // 1. Buscar función en entorno local
    auto expression = env.lookup(function_name->get_name());
    
    // 2. Si no se encuentra, buscar en global
    if (expression == nullptr) {
        extern Environment global_env;
        expression = global_env.lookup(function_name->get_name());
    }
    
    // 3. Crear nuevo entorno basado en el closure
    Environment new_env = closure->get_environment();
    
    // 4. Agregar parámetro al nuevo entorno
    new_env.add(closure->get_parameter_name(), argument_value);
    
    // 5. Evaluar cuerpo de la función en el nuevo entorno
    return closure->get_body_expression()->eval(new_env);
}
```

#### B. **LetExpression** - Entorno de Variable Local
```cpp
std::shared_ptr<Expression> LetExpression::eval(Environment& env) const {
    // 1. Evaluar expresión de la variable
    auto var_value = var_expression->eval(env);
    
    // 2. Crear copia del entorno actual
    Environment local_env = env;
    
    // 3. Agregar variable al entorno local
    local_env.add(name_expr->get_name(), var_value);
    
    // 4. Evaluar cuerpo en el entorno local
    return body_expression->eval(local_env);
}
```

#### C. **NameExpression** - Búsqueda de Variables
```cpp
std::shared_ptr<Expression> NameExpression::eval(Environment& env) const {
    // 1. Buscar en entorno local
    auto value = env.lookup(name);
    
    // 2. Si no se encuentra, buscar en global
    if (value == nullptr) {
        extern Environment global_env;
        value = global_env.lookup(name);
    }
    
    return value;
}
```

## Jerarquía de Entornos

```
┌─────────────────────────────────────┐
│           GLOBAL_ENV                │
│  ┌─────────────────────────────────┐│
│  │ Funciones definidas:            ││
│  │ - calcularPromedio              ││
│  │ - test_func                     ││
│  │ - etc.                          ││
│  └─────────────────────────────────┘│
└─────────────────────────────────────┘
                    │
                    │ (herencia)
                    ▼
┌─────────────────────────────────────┐
│         ENTORNO LOCAL               │
│  ┌─────────────────────────────────┐│
│  │ Variables locales:              ││
│  │ - mi_array = [2,2,2]            ││
│  │ - x = 5                         ││
│  │ - etc.                          ││
│  └─────────────────────────────────┘│
└─────────────────────────────────────┘
                    │
                    │ (herencia)
                    ▼
┌─────────────────────────────────────┐
│      ENTORNO DE FUNCIÓN             │
│  ┌─────────────────────────────────┐│
│  │ Parámetros de función:          ││
│  │ - notas = [2,2,2]               ││
│  │ - arr = [1,2,3]                 ││
│  │ - etc.                          ││
│  └─────────────────────────────────┘│
└─────────────────────────────────────┘
```

## Flujo de Ejecución

### Ejemplo: `let mi_array = [2,2,2] in calcularPromedio(mi_array) end`

1. **Parser** crea `LetExpression` con:
   - `var_name`: `mi_array`
   - `var_expression`: `[2,2,2]`
   - `body_expression`: `calcularPromedio(mi_array)`

2. **LetExpression::eval(global_env)**:
   - Evalúa `[2,2,2]` → `ArrayExpression`
   - Crea `local_env = global_env` (copia)
   - Agrega `mi_array → ArrayExpression` a `local_env`
   - Evalúa `calcularPromedio(mi_array)` en `local_env`

3. **CallExpression::eval(local_env)**:
   - Busca `calcularPromedio` en `local_env` → No encontrada
   - Busca en `global_env` → Encontrada (Closure)
   - Crea `new_env = closure->get_environment()` (entorno de la función)
   - Evalúa `mi_array` en `local_env` → `ArrayExpression`
   - Agrega `notas → ArrayExpression` a `new_env`
   - Evalúa cuerpo de `calcularPromedio` en `new_env`

4. **Dentro de calcularPromedio**:
   - `head(notas)` busca `notas` en `new_env` → Encontrada
   - `tail(notas)` busca `notas` en `new_env` → Encontrada
   - `calcularPromedio(resto)` crea otro entorno recursivo

## Características Clave

### 1. **Herencia de Entornos**
- Los entornos locales heredan del global
- Los entornos de función heredan del entorno donde se definió la función
- Las variables locales ocultan las globales con el mismo nombre

### 2. **Búsqueda en Cascada**
- `NameExpression` busca primero en local, luego en global
- `CallExpression` busca función primero en local, luego en global

### 3. **Entornos Inmutables**
- Los entornos se copian, no se modifican
- Cada expresión crea su propio entorno local
- No hay efectos secundarios entre entornos

### 4. **Closures**
- Las funciones capturan el entorno donde se definieron
- Permiten acceso a variables del entorno padre
- Implementan el concepto de "lexical scoping"

## Type Checking

El sistema de type checking sigue la misma jerarquía:
- `NameExpression::type_check` busca tipos en local y global
- `CallExpression::type_check` crea entornos temporales para verificar tipos
- `LetExpression::type_check` crea entornos con placeholders para verificar tipos
