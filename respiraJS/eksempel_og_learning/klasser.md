# Klasser i eiga fil : export.<funk navn> = function() { ...
For klasser i JS er det lurt å skrive fila som under. Dette gjør at funk er
tilgjengeleg utan vidare, og er god for å introdusere fil-skop!

fil world.js -- for å lage klassa world:
1#  export.world = function() {
2#      console.log('hello world');
3#  }

Da kan klasser lagast slik:
1#  var hello = require('./world');   // leitar lokalt etter 'world.js'. 
2#  hello.world();

Dei funksjonane som er tilgjengeleg direkte fra klassa kan tenkast som static-variable i klasser.

## Static factory i klassene:
Om man lagar en static factory funk i klassene, vil dette kunne gi objekter! :

klasse.js:
1#  module.exports = function() {
2#      return {    // Denne gir offentlege instansvariable til klassa
3#          factory : function(initTall) { 
4#              var private_var = initTall;
5#              return { //Public members of object
6#                  method1 : function() { return private_var; },
7#                  method2 : function() { console.log("jeje"+private_var) }
8#              }
9#          },
10#         about : function() { console.log('Mi første KLASSE'); }
11#     };
12# }();

Element å merke seg:
    - dei siste parantesane gjør at funk kalles straks når fila blir importert
    (med kommandoen):
        var myFirstClass = require('./klasse.js')


