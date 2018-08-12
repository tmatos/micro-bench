
// script matlab para gerar datasets


Fsam = 8000; // freq de amostragem

t = 0:1/Fsam:1; // 1 segundo

input = sin(2*pi*100*t) + sin(2*pi*1000*t) + sin(2*pi*3000*t); // omega = 2*pi*f

inputq = round(input * (2^29) ); // quantizar para valores na faixa de int32

fd = fopend("samples-int32.dat", "w");

fprintf(fd, "%i\n", inputq);

fclose(fd);


// plotar espectro (opcional)

//plot( 20*log10( abs( fft(input)(1:Fsam/2) ) ) );

plot( 20*log10( abs( fft(inputq)(1:Fsam/2) ) ) );
