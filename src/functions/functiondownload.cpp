/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "FunctionDownload.h"
#include "helper_macros.h"
#include <bu/plugger.h>
#include <stdlib.h>
#include <bu/sio.h>
extern "C" {
#include "http.h"
}
using namespace Bu;
PluginInterface3( pluginFunctionDownload, download, FunctionDownload, Function,
        "Ingwie Phoenix", 0, 1 );
FUNCTION_NAME(FunctionDownload, "download")  

FunctionDownload::FunctionDownload() {}
FunctionDownload::~FunctionDownload() {}

Variable FunctionDownload::call( Variable &input, VarList lParams ) {
	VarList::iterator i = lParams.begin();
	
	int sd;
	struct http_message msg;
	Bu::String url = (*i).getString();
	++i;
	Bu::String to = (*i).getString();
	Variable rt = new Variable(Variable::typeInt);

	if( (sd = http_request( url.getStr() )) < 1 ) {
		rt = 1;
		return rt;
	}

	memset( &msg, 0, sizeof( msg ) );

	while( http_response( sd, &msg ) > 0 ) {
		if( msg.content ) {
			FILE *outFile = fopen(to.getStr(),"wb");
			fwrite(msg.content, sizeof(msg.content), msg.length, outFile);
			fclose(outFile);
		}
	}
	close( sd );
	
	// Store the resulting code
	rt = msg.header.code;
	
	return rt;
}

