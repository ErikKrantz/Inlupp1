projectPKD

Git lathund (Detta är bara basics i git, finns massa fler fina funktioner som jag inte kan...)

Klona repot ner till din dator med kommandot (i terminalen alltså)
git clone https://github.com/moamarklund/projectPKD
Det skapas en mapp då på stället du står på som heter projectPKD
Koda som vanligt i en texteditor men såklart i filerna från mappen du klonade
När du har ändrat något i någon fil så pushar man upp ändringarna till repot med
git add filnamn.hs (den fil du ändrat i)
git commit -m "Skriv något om ändringen du gjorde"
git push
När man sedan ska ta ner något som någon annan ändrat gör man det med
git pull
Viktigt innan man kör pull/push är att man inte själv har ändrat i filen utan att pusha upp det alternativt inte pullat ner det senaste innan man gjort ändringar, då blir det merge (=massa dubbelkod och kommentarer som är jättejobbigt att fixa)
