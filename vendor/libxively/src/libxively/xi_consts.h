// Copyright (c) 2003-2013, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

/**
 * \file    xi_consts.h
 * \author  Olgierd Humenczuk
 * \brief   Constants that user may wish to override based on their needs
 *
 * \warning The meaning of each of these constants below is undocumented,
 *          so the user actually goes to read the source code and figure
 *          out what exactly they are doing.
 */

#ifndef __XI_CONSTST_H__
#define __XI_CONSTST_H__

#ifndef XI_HTTP_MAX_HEADERS
#define XI_HTTP_MAX_HEADERS                16
#endif

#ifndef XI_HTTP_STATUS_STRING_SIZE
#define XI_HTTP_STATUS_STRING_SIZE         32
#endif

#ifndef XI_HTTP_HEADER_NAME_MAX_SIZE
#define XI_HTTP_HEADER_NAME_MAX_SIZE       64
#endif

#ifndef XI_HTTP_HEADER_VALUE_MAX_SIZE
#define XI_HTTP_HEADER_VALUE_MAX_SIZE      64
#endif

#ifndef XI_HTTP_MAX_CONTENT_SIZE
#define XI_HTTP_MAX_CONTENT_SIZE           512
#endif

#ifndef XI_MAX_DATASTREAMS
#define XI_MAX_DATASTREAMS                 16
#endif

#ifndef XI_MAX_DATAPOINTS
#define XI_MAX_DATAPOINTS                  16
#endif

#ifndef XI_MAX_DATASTREAM_NAME
#define XI_MAX_DATASTREAM_NAME             16
#endif

#ifndef XI_VALUE_STRING_MAX_SIZE
#define XI_VALUE_STRING_MAX_SIZE           32
#endif

#ifndef XI_PRINTF_BUFFER_SIZE
#define XI_PRINTF_BUFFER_SIZE              512
#endif

#ifndef XI_QUERY_BUFFER_SIZE
#define XI_QUERY_BUFFER_SIZE               512
#endif

#ifndef XI_ID_BUFFER_SIZE
#define XI_ID_BUFFER_SIZE                  256
#endif

#ifndef XI_CONTENT_BUFFER_SIZE
#define XI_CONTENT_BUFFER_SIZE             256
#endif

#ifndef XI_CSV_BUFFER_SIZE
#define XI_CSV_BUFFER_SIZE                 128
#endif

#ifndef XI_HOST
#define XI_HOST                            "api.xively.com"
#endif

#ifndef XI_PORT
#define XI_PORT                            80
#endif


#ifndef XI_USER_AGENT
#define XI_USER_AGENT                      "libxively/0.1.x"
#endif

#endif // __XI_CONSTST_H__
