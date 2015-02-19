#include "CFG.h"

CFG *CFG::instance = 0;

CFG::CFG() {
    LIBXML_TEST_VERSION

    //Load AvrXML default
    char filename[100];
    strcpy(filename, "avrxml/ATmega328P.xml");
    this->loadAvrXML(filename);
}

CFG* CFG::getInstance() {
    if (!instance) instance = new CFG();
    return instance;
}

int CFG::getFamily() {
    return this->family;
}

VECTSTR CFG::getIOPorts() {
    return ioPorts;
}

uint8_t CFG::getAddrIO(string reg) {
    return ioRegs[reg];
}

xmlXPathObjectPtr CFG::findNode(xmlDocPtr doc, string searchstring) {
    xmlXPathContextPtr context;
	xmlXPathObjectPtr result;
	//xmlNodeSetPtr nodeset;

	context = xmlXPathNewContext(doc);

	if (context == NULL) {
		cout << "Error in xmlXPathNewContext" << endl;
		return NULL;
	}

	result = xmlXPathEvalExpression((xmlChar*)searchstring.c_str(), context);
	xmlXPathFreeContext(context);
	if (result == NULL) {
		cout << "Error in xmlXPathEvalExpression" << endl;
		return NULL;
	}

	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
        cout << "No result" << endl;
		return NULL;
	}
	return result;

/*
	context = xmlXPathNewContext(doc);
	result = xmlXPathEvalExpression((xmlChar*)searchstring.c_str(), context);
	nodeset = result->nodesetval;
	if (xmlXPathNodeSetIsEmpty(nodeset)) {
        xmlXPathFreeObject(result);
        cout << "Failed to find " << endl;
	}
	*/

    //xmlNodePtr resp = nodeset->nodeTab[0];
    //xmlXPathFreeObject (result);

    //return result;
}

void CFG::loadAvrXML(char* filename) {
    xmlDocPtr avrxml;
    xmlNodePtr p;
    xmlXPathObjectPtr result;
    xmlNodeSetPtr nodeset;
    xmlChar *prop1, *prop2;
    int i;
    uint8_t value;

    avrxml = xmlReadFile(filename, NULL, 0);
    p = xmlDocGetRootElement(avrxml);
    if (p == NULL) {
        xmlFreeDoc(avrxml);
        cout << "Failed to parse " << filename << endl;
        return;
    }

    ioPorts.clear();
    ioRegs.clear();

    //AVR Family
    string searchstring =  "//device/@family";
    result = findNode(avrxml, searchstring);
    nodeset = result->nodesetval;
    p = nodeset->nodeTab[0];
    if(!xmlStrcmp( p->children->content, (const xmlChar*)"megaAVR" ) ){
        family = CFG_MEGA;
    } else {
        if(!xmlStrcmp( p->children->content, (const xmlChar*)"tinyAVR" ) ){
            family = CFG_TINY;
        } else {
            if(!xmlStrcmp( p->children->content, (const xmlChar*)"XMega" ) ){
                family = CFG_XMEGA;
            } else {
                family = CFG_AVR;
            }
        }
    }
    xmlXPathFreeObject (result);

    //PORTS
    string pname;
    searchstring =  "//module/register-group[contains(@name,'PORT')]";
    result = findNode(avrxml, searchstring);
    nodeset = result->nodesetval;
    for (i=0; i < nodeset->nodeNr; i++) {
        p = nodeset->nodeTab[i];

        prop1 = xmlGetProp(p, (const xmlChar *)"name");
        pname = (const char*)prop1;
        ioPorts.push_back(pname.substr(4, string::npos)); //Insert just a letter of port's name.
        free(prop1);

        p = p->children;
        while (p != NULL) {
            if(p->type == XML_ELEMENT_NODE) {
                prop1 = xmlGetProp(p, (const xmlChar *)"name");
                prop2 = xmlGetProp(p, (const xmlChar *)"offset");
                value = strtol((const char*)prop2, NULL, 16) - 0x20;

                ioRegs[(const char*)prop1] = value;

                free(prop1);
                free(prop2);
            }
            p = p->next;
        }
    }
    xmlXPathFreeObject (result);



/*
        xmlAttr* attribute = p->properties;
        while(attribute && attribute->name && attribute->children) {
          //xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
          //do something with value
          //xmlFree(value);
          cout << attribute->name << endl;
          attribute = attribute->next;
        }
*/
        //prop = xmlGetProp(p, (const xmlChar *)"offset");
        //cout << prop << endl;
       // free(prop);
        //prop = xmlGetProp(p, (const xmlChar *)"offset");
        //cout << prop << endl;
       // free(prop);




    // printf ("%d, %d, %d\n", mapIO["PORTB"], mapIO["DDRB"], mapIO["PINB"]);
    // printf ("%d, %d, %d\n", mapIO["PORTC"], mapIO["DDRC"], mapIO["PINC"]);
    // printf ("%d, %d, %d\n", mapIO["PORTD"], mapIO["DDRD"], mapIO["PIND"]);

//xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);

    //cout << (char *)attr->children->content << endl;

    //xmlChar *prop = xmlGetProp(p, (const xmlChar *)"size");
    //cout << prop << endl;
    //free(prop);


    /*
    p = p->xmlChildrenNode;
	while (p != NULL) {
	    if ((!xmlStrcmp(p->name, (const xmlChar *)"memory-segment"))) {
		    prop = xmlGetProp(p, (const xmlChar *)"size");
		    cout << prop << endl;
		    free(prop);
 	    }
	    p = p->next;

	*/

    xmlFreeDoc(avrxml);
    xmlCleanupParser();
}
