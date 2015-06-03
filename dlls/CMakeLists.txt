
set(SRC_HL_DLL
	airtank.cpp
	animating.cpp
	animation.cpp
	bmodels.cpp
	buttons.cpp
	cbase.cpp
	client.cpp
	combat.cpp
	defaultai.cpp
	doors.cpp
	effects.cpp
	explode.cpp
	func_break.cpp
	func_tank.cpp
	game.cpp
	gamerules.cpp
	globals.cpp
	healthkit.cpp
	h_ai.cpp
	h_battery.cpp
	h_cycler.cpp
	h_export.cpp
	items.cpp
	lights.cpp
	maprules.cpp
	mortar.cpp
	multiplay_gamerules.cpp
	nodes.cpp
	observer.cpp
	pathcorner.cpp
	plane.cpp
	plats.cpp
	player.cpp
	schedule.cpp
	scripted.cpp
	singleplay_gamerules.cpp
	skill.cpp
	sound.cpp
	soundent.cpp
	spectator.cpp
	subs.cpp
	teamplay_gamerules.cpp
	triggers.cpp
	util.cpp
	world.cpp
	xen.cpp
	monsters/aflock.cpp
	monsters/agrunt.cpp
	monsters/apache.cpp
	monsters/barnacle.cpp
	monsters/barney.cpp
	monsters/bigmomma.cpp
	monsters/bloater.cpp
	monsters/bullsquid.cpp
	monsters/controller.cpp
	monsters/flyingmonster.cpp
	monsters/gargantua.cpp
	monsters/genericmonster.cpp
	monsters/gman.cpp
	monsters/h_cine.cpp
	monsters/hassassin.cpp
	monsters/headcrab.cpp
	monsters/hgrunt.cpp
	monsters/houndeye.cpp
	monsters/ichthyosaur.cpp
	monsters/islave.cpp
	monsters/leech.cpp
	monsters/monstermaker.cpp
	monsters/monsters.cpp
	monsters/monsterstate.cpp
	monsters/nihilanth.cpp
	monsters/osprey.cpp
	monsters/rat.cpp
	monsters/roach.cpp
	monsters/scientist.cpp
	monsters/squadmonster.cpp
	monsters/talkmonster.cpp
	monsters/tempmonster.cpp
	monsters/tentacle.cpp
	monsters/turret.cpp
	monsters/zombie.cpp
	weapons/crossbow.cpp
	weapons/crowbar.cpp
	weapons/egon.cpp
	weapons/gauss.cpp
	weapons/ggrenade.cpp
	weapons/glock.cpp
	weapons/handgrenade.cpp
	weapons/hornet.cpp
	weapons/hornetgun.cpp
	weapons/mp5.cpp
	weapons/python.cpp
	weapons/rpg.cpp
	weapons/satchel.cpp
	weapons/shotgun.cpp
	weapons/squeakgrenade.cpp
	weapons/tripmine.cpp
	weapons/weapons.cpp
	../cl_dll/ranking.cpp
	../game_shared/voice_gamemgr.cpp
	../pm_shared/pm_debug.c
	../pm_shared/pm_math.c
	../pm_shared/pm_shared.c
)

set(HDR_HL_DLL
	../common/const.h
	../common/entity_state.h
	../common/hltv.h
	../common/in_buttons.h
	../common/mathlib.h
	../common/netadr.h
	../common/pmtrace.h
	../common/studio_event.h
	../common/usercmd.h
	../common/vector.h
	../common/weaponinfo.h
	../common/winsani_in.h
	../common/winsani_out.h
	activity.h
	activitymap.h
	animation.h
	monsters/basemonster.h
	cbase.h
	cdll_dll.h
	client.h
	decals.h
	defaultai.h
	doors.h
	effects.h
	enginecallback.h
	explode.h
	extdll.h
	monsters/flyingmonster.h
	func_break.h
	game.h
	gamerules.h
	weapons/hornet.h
	items.h
	maprules.h
	monsters/monsterevent.h
	monsters/monsters.h
	nodes.h
	plane.h
	player.h
	saverestore.h
	schedule.h
	scripted.h
	scriptevent.h
	skill.h
	soundent.h
	spectator.h
	monsters/squadmonster.h
	monsters/talkmonster.h
	teamplay_gamerules.h
	trains.h
	util.h
	weapons/weapons.h
	../cl_dll/ranking.h
	../engine/crc.h
	../engine/custom.h
	../engine/customentity.h
	../engine/cvardef.h
	../engine/edict.h
	../engine/eiface.h
	../engine/event_flags.h
	../engine/nowin.h
	../engine/progdefs.h
	../engine/Sequence.h
	../engine/shake.h
	../engine/studio.h
	../game_shared/bitvec.h
	../game_shared/voice_common.h
	../game_shared/voice_gamemgr.h
	../pm_shared/pm_debug.h
	../pm_shared/pm_defs.h
	../pm_shared/pm_info.h
	../pm_shared/pm_materials.h
	../pm_shared/pm_movevars.h
	../pm_shared/pm_shared.h
	../public/archtypes.h
	../public/steam/steamtypes.h
)

add_library(game SHARED ${SRC_HL_DLL} ${HDR_HL_DLL})

include_directories(
	../common
	../dlls
	../dlls/monsters
	../dlls/weapons
	../engine
	../game_shared
	../pm_shared
	../public
	../lib/curl/include
)

add_definitions(-DCLIENT_WEAPONS -DCURL_STATICLIB)

find_file(CURL_LIBRARY libcurl.lib ../lib/curl/lib)

message(STATUS "Found curl library @ ${CURL_LIBRARY}")

set_target_properties(game PROPERTIES LINK_FLAGS "/DEF:\"${CMAKE_SOURCE_DIR}/dlls/hl.def\" /NODEFAULTLIB:mfc110d")

target_link_libraries(game ${CURL_LIBRARY})

if(DEFINED MOD_FOLDER)
    if(NOT DEFINED MOD_DLL_INSTALL_FOLDER)
        set(MOD_DLL_INSTALL_FOLDER "${MOD_FOLDER}\\dlls")
    endif(NOT DEFINED MOD_DLL_INSTALL_FOLDER)

	message(STATUS "Will copy game to ${MOD_DLL_INSTALL_FOLDER}")
    add_custom_target(Copy_HL_DLL_To_Mod_Directory ALL
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}\\dlls\\game.dll" ${MOD_DLL_INSTALL_FOLDER}
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}\\dlls\\game.pdb" ${MOD_DLL_INSTALL_FOLDER}
    )
	add_dependencies(Copy_HL_DLL_To_Mod_Directory game)
endif(DEFINED MOD_FOLDER)
