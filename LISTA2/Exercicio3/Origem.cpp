/*
Agora quando os objetos são posicionados na cena sua posição está numa proporção 1 pra 1 em relação à janela, portanto para desenhar as mesmas formas
de antes as informações de coordenadas devem ser reconfiguradas de acordo. Essa configuração é útil pois é muito mais simples tratar coordenadas com
números que referenciam o tamanho da tela do que o sistema de -1.0 a 1.0 que, basicamente, trata as coordenadas de acordo com qual a porcentagem que
o vértice está deslocado em relação ao centro da janela. O sistema de coordenadas que segue o tamanho da janela é o padrão no desenvolvimento de jogos
justamente pela simplicidade que existe em tratar as coordenadas dessa forma.


*/