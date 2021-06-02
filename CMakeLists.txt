cmake_minimum_required (VERSION 3.14)
set(CRYENGINE_DIR "C:/Program Files (x86)/Crytek/CRYENGINE Launcher/Crytek/CRYENGINE_5.6" CACHE STRING "CRYENGINE root directory.")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../")

include("${TOOLS_CMAKE_DIR}/InitialSetup.cmake")
include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Root"
		"FlashModule.cpp"
		"GameEvents.cpp"
		"GamePlugin.cpp"
		"StdAfx.cpp"
		"FlashModule.h"
		"GameEvents.h"
		"GamePlugin.h"
		"IGameEvents.h"
		"StdAfx.h"
)
add_sources("Components_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Components"
		"Components/ControllableTimer.cpp"
		"Components/EntityLinks.cpp"
		"Components/ControllableTimer.h"
		"Components/EntityLinks.h"
)
add_sources("Enemies_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Components\\\\Enemies"
		"Components/Enemies/Enemy.cpp"
		"Components/Enemies/EnemySpawner.cpp"
		"Components/Enemies/SpawnExcluder.cpp"
		"Components/Enemies/Enemy.h"
		"Components/Enemies/EnemySpawner.h"
		"Components/Enemies/SpawnExcluder.h"
)
add_sources("Flash_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Components\\\\Flash"
		"Components/Flash/DynamicComponent.cpp"
		"Components/Flash/DynamicFunctions.cpp"
		"Components/Flash/ElementFunctions.cpp"
		"Components/Flash/Signals.cpp"
		"Components/Flash/DynamicComponent.h"
		"Components/Flash/DynamicFunctions.h"
		"Components/Flash/ElementFunctions.h"
		"Components/Flash/Signals.h"
)
add_sources("Game_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Components\\\\Game"
		"Components/Game/Damage.cpp"
		"Components/Game/EventSubscriber.cpp"
		"Components/Game/Health.cpp"
		"Components/Game/PlayArena.cpp"
		"Components/Game/Damage.h"
		"Components/Game/EventSubscriber.h"
		"Components/Game/Health.h"
		"Components/Game/PlayArena.h"
)
add_sources("Physics_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Components\\\\Physics"
		"Components/Physics/CollisionMask.cpp"
		"Components/Physics/Explosion.cpp"
		"Components/Physics/ParticleDisplacement.cpp"
		"Components/Physics/CollisionMask.h"
		"Components/Physics/Explosion.h"
		"Components/Physics/ParticleDisplacement.h"
)
add_sources("Player_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Components\\\\Player"
		"Components/Player/Player.cpp"
		"Components/Player/PlayerController.cpp"
		"Components/Player/PlayerCursor.cpp"
		"Components/Player/PlayerSpawnPoint.cpp"
		"Components/Player/Player.h"
		"Components/Player/PlayerController.h"
		"Components/Player/PlayerCursor.h"
		"Components/Player/PlayerSpawnPoint.h"
)
add_sources("Weapons_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Components\\\\Weapons"
		"Components/Weapons/Projectile.cpp"
		"Components/Weapons/Weapon.cpp"
		"Components/Weapons/WeaponAttachment.cpp"
		"Components/Weapons/Projectile.h"
		"Components/Weapons/Weapon.h"
		"Components/Weapons/WeaponAttachment.h"
)
add_sources("Env_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Env"
		"Env/DynamicRegistry.cpp"
		"Env/FlashRegistry.cpp"
		"Env/GameEnv.cpp"
		"Env/DynamicRegistry.h"
		"Env/EnvTypes.h"
		"Env/FlashRegistry.h"
		"Env/FlashTypes.h"
		"Env/GameEnv.h"
)
add_sources("Elements_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Env\\\\Elements"
		"Env/Elements/EnvDynamicComponent.h"
		"Env/Elements/EnvDynamicFunction.h"
		"Env/Elements/EnvDynamicSignal.h"
		"Env/Elements/EnvFlashComponent.h"
)
add_sources("Nodes_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Env\\\\Nodes"
		"Env/Nodes/GameFunctions.cpp"
		"Env/Nodes/GameFunctions.h"
)
add_sources("Utils_uber.cpp"
    PROJECTS GalaxSys
    SOURCE_GROUP "Env\\\\Utils"
		"Env/Utils/FlashUtils.h"
		"Env/Utils/Utils.h"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CVarOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "CVarOverrides.h"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EngineDefineOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "EngineDefineOverrides.h"
    )
endif()
end_sources()

CryGameModule(GalaxSys FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-variable
        -Wno-reorder
        -Wno-unknown-pragmas
        -Wno-parentheses
        -Wno-switch
        -Wno-format
        -Wno-dynamic-class-memaccess
        -Wno-unused-private-field
        -Wno-unused-value
        -Wno-invalid-offsetof
        -Wno-multichar
        -Wno-char-subscripts
        -Wno-null-conversion
        -Wno-empty-body
        -Wno-unused-lambda-capture
        -Wno-unused-function
        -Wno-tautological-constant-out-of-range-compare
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-function
        -Wno-unused-value
        -Wno-unused-variable
        -Wno-aligned-new
        -Wno-int-in-bool-context
        -Wno-invalid-offsetof
        -Wno-comment
        -Wno-sign-compare
        -Wno-unused-but-set-variable
        -Wno-maybe-uninitialized
        -Wno-misleading-indentation
        -Wno-unused-result
        -Wno-unknown-pragmas
        -Wno-multichar
        -Wno-strict-aliasing
        -Wno-char-subscripts
        -Wno-conversion-null
        -Wno-reorder
        -Wno-parentheses
        -Wno-format
        -Wno-switch
    )
endif()


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
        add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
        set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
        if (WIN32)
            set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/GalaxSys.cryproject\"")
        endif()
    endif()
else()
    add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/GalaxSys.cryproject\"")
    endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/GalaxSys.cryproject\"")
    endif()

    add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"${PROJECT_DIR}/GalaxSys.cryproject\"")
    endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/GalaxSys.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
#END-CUSTOM