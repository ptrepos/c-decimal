/**
 * Copyright (C) dachicraft foundation 2015
 * @author t.hada 2011/03/15 
 *
 * MG_API_EXPORT 	外部ライブラリ シンボル出力
 * MG_API_IMPORT 	外部ライブラリ シンボル取込
 * MG_API_PRIVATE 	外部ライブラリ ファイル作成時のシンボル隠蔽指定
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MG_API_EXPORT
#	if defined(_WIN32) && defined(_MSC_VER)
#		define MG_API_EXPORT	__declspec(dllexport)
#	elif defined(_WIN32) && (__GNUC__ >= 4 || defined(__clang__))
#		define MG_API_EXPORT	__attribute__ ((dllexport))
#	elif __GNUC__ >= 4 || defined(__clang__)
#		define MG_API_EXPORT
#	else
#		error "Not supported architecture"
#	endif
#endif

#ifndef MG_API_IMPORT
#	if defined(_WIN32) && defined(_MSC_VER)
#		define MG_API_IMPORT	__declspec(dllimport)
#	elif defined(_WIN32) && (__GNUC__ >= 4 || defined(__clang__))
#		define MG_API_IMPORT	__attribute__ ((dllimport))
#	elif __GNUC__ >= 4 || defined(__clang__)
#		define MG_API_IMPORT
#	else
#		error "Not supported architecture"
#	endif
#endif

#ifndef MG_PRIVATE
#	if defined(_MSC_VER)
#		define MG_PRIVATE	
#	elif __GNUC__ >= 4 || defined(__clang__)
#		define MG_PRIVATE	__attribute__ ((visibility ("hidden")))
#	else
#		define MG_PRIVATE	
#	endif
#endif

#ifdef __cplusplus
}
#endif
