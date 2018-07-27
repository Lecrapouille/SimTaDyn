<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">
  <xsl:template match="/">

    <html>
      <body>

        <h3>MySQL2XML XSL Stylesheet - Projet UltraSimtadyn - B.M.</h3>

        <table border="1" cellspacing="0" cellpadding="3">
          <tr bgcolor="#FFFF00">
            <td>Id</td>
            <td>x</td>
            <td>y</td>
            <td>z</td>
            <td>name</td>
            <td>R</td>
            <td>G</td>
            <td>B</td>
            <td>A</td>
            <td>mforth</td>
          </tr>
          <xsl:for-each select="root/graphe-sommet/sommet">
            <tr>
              <td><xsl:value-of select="id"/></td>
              <td><xsl:value-of select="x"/></td>
              <td><xsl:value-of select="y"/></td>
              <td><xsl:value-of select="z"/></td>
              <td><xsl:value-of select="name"/></td>
              <td><xsl:value-of select="R"/></td>
              <td><xsl:value-of select="G"/></td>
              <td><xsl:value-of select="B"/></td>
	      <td><xsl:value-of select="A"/></td>
              <td><xsl:value-of select="mforth"/></td>
            </tr>
          </xsl:for-each>
        </table>

        <table border="1" cellspacing="0" cellpadding="3">
          <tr bgcolor="#FFFF00">
            <td>Id</td>
            <td>x</td>
            <td>y</td>
            <td>z</td>
            <td>name</td>
            <td>R</td>
            <td>G</td>
            <td>B</td>
	    <td>A</td>
	    <td>cout</td>
            <td>id_som_1</td>
            <td>id_som_2</td>
            <td>mforth</td>
	    <td>triangle</td>
          </tr>
          <xsl:for-each select="root/graphe-arc/arc">
            <tr>
              <td><xsl:value-of select="id"/></td>
              <td><xsl:value-of select="x"/></td>
              <td><xsl:value-of select="y"/></td>
              <td><xsl:value-of select="z"/></td>
              <td><xsl:value-of select="name"/></td>
              <td><xsl:value-of select="R"/></td>
              <td><xsl:value-of select="G"/></td>
              <td><xsl:value-of select="B"/></td>
	      <td><xsl:value-of select="A"/></td>
	      <td><xsl:value-of select="cout"/></td>
              <td><xsl:value-of select="id_som_1"/></td>
              <td><xsl:value-of select="id_som_2"/></td>
              <td><xsl:value-of select="mforth"/></td>
	      <td><xsl:value-of select="triangle"/></td>
            </tr>
          </xsl:for-each>
        </table>

        <table border="1" cellspacing="0" cellpadding="3">
          <tr bgcolor="#FFFF00">
            <td>Id</td>
            <td>x</td>
            <td>y</td>
            <td>z</td>
            <td>name</td>
            <td>R</td>
            <td>G</td>
            <td>B</td>
            <td>A</td>
	    <td>nbr_arete</td>
	    <td>liste_arete</td>
            <td>mforth</td>
          </tr>
          <xsl:for-each select="root/graphe-zone/zone">
            <tr>
              <td><xsl:value-of select="id"/></td>
              <td><xsl:value-of select="x"/></td>
              <td><xsl:value-of select="y"/></td>
              <td><xsl:value-of select="z"/></td>
              <td><xsl:value-of select="name"/></td>
              <td><xsl:value-of select="R"/></td>
              <td><xsl:value-of select="G"/></td>
              <td><xsl:value-of select="B"/></td>
              <td><xsl:value-of select="A"/></td>
              <td><xsl:value-of select="nbar"/></td>
              <td><xsl:value-of select="listar"/></td>
              <td><xsl:value-of select="mforth"/></td>
            </tr>
          </xsl:for-each>
        </table>

      </body>  
    </html>
  
  </xsl:template>
</xsl:stylesheet>
