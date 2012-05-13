<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output
        method="html"
        doctype-public="XSLT-compat"
        omit-xml-declaration="yes"
        encoding="UTF-8"
        indent="yes" />
    <xsl:template match="log">
        <html>
            <head>
                <title>Pythonic Universe logfile</title>
                <style type="text/css">
body {
    font-family: Cantarell;
}

table {
    border-collapse: collapse;
    width: 100%;
}

td, th {
    border-top: 1px dashed #808080;
    border-bottom: 1px dashed #808080;
    padding: 0.1em;
    padding-left: 0.2em;
    padding-right: 0.2em;
}

td {
    font-family: monospace;
    vertical-align: top;
}

th {
    font-weight: bold;
    text-align: left;
}

.hint {
    background-color: hsl(280, 100%, 80%);
}

.info {
    background-color: hsl(240, 100%, 90%);
}

.warn {
    background-color: hsl(60, 100%, 80%);
}

.error {
    background-color: hsl(0, 100%, 80%);
}

.panic {
    background-color: hsl(15, 100%, 80%);
}

tr.panic td {
    font-weight: bold;
}

tr:hover td {
    background-color: rgba(0, 0, 0, 0.3);
}

ul.legend {
    list-style-type: none;
    margin-left: 0;
    margin-right: 0;
    padding: 0;
}

ul.legend:before {
    content: "Legend:";
    font-weight: bold;
}

ul.legend li {
    display: inline;
    margin-left: 0.25em;
    margin-right: 0.25em;
    padding: 0;
    padding-left: 0.2em;
    padding-right: 0.2em;
}
                </style>
            </head>
            <body>
                <h1>Pythonic Universe logfile</h1>
                <p>This is a log file belonging to <a href="http://pyuniverse.org">Pythonic Universe</a>.</p>
                <ul class="legend">
                    <li class="debug">debug</li>
                    <li class="info">info</li>
                    <li class="hint">hint</li>
                    <li class="warn">warn</li>
                    <li class="error">error</li>
                    <li class="panic">panic</li>
                </ul>
                <table>
                    <colgroup>
                        <col class="timestamp"></col>
                    </colgroup>
                    <thead>
                        <tr>
                            <th>Timestamp</th>
                            <th>Severity</th>
                            <th>Subsystem</th>
                            <th>Message</th>
                        </tr>
                    </thead>
                    <tbody>
                        <xsl:for-each select="message">
                            <tr>
                                <xsl:attribute name="class"><xsl:value-of select="severity" /></xsl:attribute>
                                <td class="timestamp"><xsl:value-of select="timestamp" /></td>
                                <td class="severity"><xsl:value-of select="severity" /></td>
                                <td class="channel"><xsl:value-of select="channel" /></td>
                                <td class="text"><xsl:value-of select="text" /></td>
                            </tr>
                        </xsl:for-each>
                    </tbody>
                </table>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
