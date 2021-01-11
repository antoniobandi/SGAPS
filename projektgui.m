clear variables;clc;

t = tcpclient("192.168.4.1", 8080, 'ConnectTimeout', 15);
configureTerminator(t,"CR/LF");

create_gui(t);

flush(t);
while (t.NumBytesAvailable < 1000)
    continue;
end
readline(t);
readline(t);
time = 0:0.2:12.6;
a = 0 * time;
plot(time,a);
movegui([895 500]);
grid on;
grid minor;

while 1
    wloop(t,time);
end




function wloop(t,time)
    while t.NumBytesAvailable > 0
        a = char(readline(t));
        while (length(a) ~= length(time)*4)
            a = char(readline(t));
        end
%         a = read(t,256);
%         read(t,1);
        a = typecast(uint8(a),'single');
        plot(time,a);
        grid on;
        grid minor;
    end
    
    while t.NumBytesAvailable == 0
    end
end

function create_gui(t)
    fig = uifigure;
    fig.Name = 'Izbor';
%     fig.Color = [0.01 0.37 0.24];
    fig.Color = 'w';
    fig.Resize = 'off';
    fig.Position = [100 500 800 500];
    
    stop_btn = uibutton(fig,'Text','Quit','Position',[350 15 100 22],'FontWeight','bold','BackgroundColor',[1 0.2 0],'FontSize',13);
    
    gen_bg = uibuttongroup(fig,'Title','Generator','FontSize',13,'TitlePosition','centertop','Position',[0 360 150 120]);
    gen_on = uiradiobutton(gen_bg,'Text','on','Position',[55 65 60 20]); %#ok<*NASGU>
    gen_off = uiradiobutton(gen_bg,'Text','off','Position',[55 30 60 20],'Value',true);
    
    pass_bg = uibuttongroup(fig,'Title','Low/High Pass','FontSize',13,'TitlePosition','centertop','Position',[340 90 150 120]);
    lpass_rb = uiradiobutton(pass_bg,'Text','low','Position',[55 65 60 20], 'Value',true);
    hpass_rb = uiradiobutton(pass_bg,'Text','high','Position',[55 30 60 20]);
    
    specPC_bg = uibuttongroup(fig,'Title','Signal/Spectrum PC','FontSize',13,'TitlePosition','lefttop','Position',[0 90 150 120]);
    sigPC_tb = uitogglebutton(specPC_bg,'Position',[10 50 100 22],'Value',1,'Text','Signal');
    specPC_tb = uitogglebutton(specPC_bg,'Position',[10 28 100 22],'Text','Spectrum');
    
    specLED_bg = uibuttongroup(fig,'Title','Signal/Spectrum LED','FontSize',13,'TitlePosition','lefttop','Position',[170 90 150 120]);
    sigLED_tb = uitogglebutton(specLED_bg,'Position',[10 50 100 22],'Value',1,'Text','Signal');
    specLED_tb = uitogglebutton(specLED_bg,'Position',[10 28 100 22],'Text','Spectrum');
    
    dropdown_pan = uipanel('Parent',fig,'Title','Oblik signala','FontSize',13,'Position',[170 360 200 120]);
    dropd = uidropdown('Parent',dropdown_pan,'Items',{'Sinusni','Pravokutni'},'Position',[20 45 160 25]);
    
    fg_pan = uipanel('Parent',fig,'Title','Granična frekvencija','FontSize',13,'Position',[510 90 285 120]);
    fg_drop = uidropdown('Parent',fg_pan,'Items',{'0.1','0.2','0.3','0.4','0.5','0.6','0.7','0.8','0.9'},'Value','0.5','Position',[20 45 160 25]);
    
    amp_pan = uipanel('Parent',fig,'Title','Amplituda signala','FontSize',13,'Position',[390 360 405 120]);
    amp_slider = uislider('Parent',amp_pan,'Limits',[0 4000],'Value', 2000,'Position',[10 60 380 3],'FontSize',9);
    amp_slider.MajorTicks = [0 200 400 600 800 1000 1200 1400 1600 1800 2000 2200 2400 2600 2800 3000 3200 3400 3600 3800 4000];
    amp_slider.MajorTickLabels = {'0' '0.1' '0.2' '0.3' '0.4' '0.5' '0.6' '0.7' '0.8' '0.9' '1' '1.1' '1.2' '1.3' '1.4' '1.5' '1.6' '1.7' '1.8' '1.9' '2'};
    
    freq_pan = uipanel('Parent',fig,'Title','Frekvencija signala','FontSize',13,'Position',[5 225 793 120]);
    freq_slider = uislider('Parent',freq_pan,'Limits',[100 1000],'Value', 500,'Position',[15 60 760 3],'FontSize',9);
    
    stop_btn.ButtonPushedFcn = {@stopfcn,t};
    gen_bg.SelectionChangedFcn = {@gen_selection,t};
    specPC_bg.SelectionChangedFcn = {@specPC_selection,t};
    specLED_bg.SelectionChangedFcn = {@specLED_selection,t};
    pass_bg.SelectionChangedFcn = {@pass_selection,t};
    dropd.ValueChangedFcn = {@wave_selection,t};
    fg_drop.ValueChangedFcn = {@fg_selection,t};
    freq_slider.ValueChangedFcn = {@freq_selection,t};
    amp_slider.ValueChangedFcn = {@amp_selection,t};
end

function gen_selection(~,event,client)
    writeline(client,strcat("G:",string(event.NewValue.Text)));
    fprintf("%s\n", strcat("Gen:",string(event.NewValue.Text)));
%     fprintf("%d\n", client.NumBytesWritten);
%     fprintf("----------\n");
end

function wave_selection(~,event,client)
    writeline(client,strcat("W:",string(event.Value)));
    fprintf("%s\n",strcat("Wave:",string(event.Value)));
%     fprintf("%d\n", client.NumBytesWritten);
%     fprintf("----------\n");
    
end

function fg_selection(~,event,client)
    writeline(client,strcat("U:",string(event.Value)));
    fprintf("%s\n",strcat("U:",string(event.Value)));
%     fprintf("%d\n", client.NumBytesWritten);
%     fprintf("----------\n");
    
end

function freq_selection(~,event,client)
    writeline(client,strcat("F:0",string(round(event.Value))));
    fprintf("Freq:%s\n", string(round(event.Value)));
%     fprintf("%d\n", client.NumBytesWritten);
%     fprintf("----------\n");
    
end

function amp_selection(~,event,client)
    writeline(client,strcat("A:0",string(round(event.Value))));
    fprintf("Amp:%s\n", string(round(event.Value)));
%     fprintf("%d\n", client.NumBytesWritten);
%     fprintf("----------\n");    
    
end

function pass_selection(~,event,client)
    writeline(client,strcat("P:",string(event.NewValue.Text)));
    fprintf("%s\n", strcat("Pass:",string(event.NewValue.Text)));
%     fprintf("%d\n", client.NumBytesWritten);
%     fprintf("----------\n");
end

function specPC_selection(~,event,client)
    writeline(client,strcat("S:",string(event.NewValue.Text)));
    fprintf("%s\n", strcat("S:",string(event.NewValue.Text)));
end

function specLED_selection(~,event,client)
    writeline(client,strcat("L:",string(event.NewValue.Text)));
    fprintf("%s\n", strcat("L:",string(event.NewValue.Text)));
end

function stopfcn(~,~,client)
    writeline(client,"Q");
    exit();
end