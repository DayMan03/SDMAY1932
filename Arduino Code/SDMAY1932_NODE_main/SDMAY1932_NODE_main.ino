/*
  Senior Design Team F18/S19: SDMAY19-32
  Author: Francisco Alegria
  Start Date: 2-23-19
  Project: Hardware synthesizer

*/
////////////////////////////////////////////////////
// Libraries 
  #include <ESP8266WiFi.h>
  #include <WiFiUdp.h>
  #include <OSCMessage.h>
  #include <OSCBundle.h>
  #include <Wire.h>
////////////////////////////////////////////////////
// I2C Variables 
  #define SCL 4
  #define SDA 5

  int i2cMux = 1;
////////////////////////////////////////////////////
// Wifi Variables 
  //char ssid[] = "IASTATE";       //iastate network: IASTATE
  //char pass[] = ""; 
  
  char ssid[] = "CenturyLink9713"; //paco CR apt
  char pass[] = "4efda8d46777v3";  
           

  WiFiUDP Udp;
  
  //const IPAddress destIpad(10,26,42,73); //iPad at iastate network: IASTATE
  const IPAddress destIpad(192,168,0,4); //iPad at paco CR apt
  //const IPAddress destIphone(192,168,0,3); //iPhone at paco CR apt
  
  const unsigned int destPort = 9000;   // OSC port ipad receive
  const unsigned int localPort = 8000;  // OSC port node receive

  char incomingPacket[255]; //debug variable for incoming label detection

  OSCErrorCode errCode;
////////////////////////////////////////////////////
//SYNTH Variables 
  int patch = 0;
  //int patchC[132];
  int patch0[132];
  int patch1[132];
  int patch2[132];
  int patch3[132];
  int patch4[132];
  int patch5[132];
  int patch6[132];
  int patch7[132];
  int patch8[132];

  int page = 1;

  // page 1
  // osc 1
  float coarse1 = 0.5;
  float coarse1val = 0;
  int oct1val = 2;
  float pwm1 = 0.5;
  int pwm1val = 50;
  float wave1 = 0;
  int wave1val = 0;

  // osc 2
  float coarse2 = 0.5;
  float coarse2val = 0;
  int oct2val = 2;
  float pwm2 = 0.5;
  int pwm2val = 50;
  float wave2 = 0;
  int wave2val = 1;

  // lfo 1
  float rate1 = 0;
  int rate1val = 0;
  float pwm1lfo = 0.5;
  int pwm1lfoVal = 50;
  float wave3 = 0;
  int wave3val = 1;

  // lfo 2
  float rate2 = 0;
  int rate2val = 0;
  float pwm2lfo = 0.5;
  int pwm2lfoVal = 50;
  float wave4 = 0;
  int wave4val = 1;
  
  // adsr 1
  float adsr11 = 0.0;
  float adsr12 = 0.0;
  float adsr13 = 0.0;
  float adsr14 = 0.0;

  // adsr 2
  float adsr21 = 0.0;
  float adsr22 = 0.0;
  float adsr23 = 0.0;
  float adsr24 = 0.0;

  // mixer
  float mixer1 = 0;
  float mixer2 = 0;
  float mixer3 = 0;
  float mixer4 = 0;
  float mixer5 = 0;
  float mixer6 = 0;

  // output amp
  float master = 0;
  int masterVal = 0;
  bool mute = 0;
  bool hold = 0;

  // preset patches
  float save = 0;
  int patchVal = 0;
  float patchP = 0;
  float patchN = 0;

  // filters
  float filter = 0;
  int filterType = 1;
  float lpf = 0;
  int lpfVal = 0;
  float hpf = 0;
  int hpfVal = 0;

  // XY touch 'keyboard'
  float XYmode = 0;
  int XYMp1 = 1;
  float XYp1val1x = 0;
  float XYp1val1y = 0;
  float XYp1val2x = 0;
  float XYp1val2y = 0;

  // MIDI notes
  int noteIn1 = 129;
  int noteIn2 = 129;
  bool noteOn1 = 0;
  bool noteOn2 = 0;
  //float vel1 = 0;
  //float vel2 = 0;
  

////////////////////////////////////////////////////
// GUI control label address variables
char coarse1lb[] = "/1/coarse1val";
char oct1lb[] = "/1/oct1val";
char pwm1lb[] = "/1/pwm1val";
//char wave1lb[] = "/1/wave1val";
char sin1lb[] = "/1/osc1sin";
char tri1lb[] = "/1/osc1tri";
char saw1lb[] = "/1/osc1saw";
char pls1lb[] = "/1/osc1pls";
char coarse2lb[] = "/1/coarse2val";
char oct2lb[] = "/1/oct2val";
char pwm2lb[] = "/1/pwm2val";
//char wave2lb[] = "/1/wave2val";
char sin2lb[] = "/1/osc2sin";
char tri2lb[] = "/1/osc2tri";
char saw2lb[] = "/1/osc2saw";
char pls2lb[] = "/1/osc2pls"; 
char rate1lb[] = "/1/rate1val";
char pwm1lfolb[] = "/1/pwm1lfoVal";
char sin3lb[] = "/1/lfo1sin";
char tri3lb[] = "/1/lfo1tri";
char saw3lb[] = "/1/lfo1saw";
char pls3lb[] = "/1/lfo1pls";
char rate2lb[] = "/1/rate2val";
char pwm2lfolb[] = "/1/pwm2lfoVal";
char sin4lb[] = "/1/lfo2sin";
char tri4lb[] = "/1/lfo2tri";
char saw4lb[] = "/1/lfo2saw";
char pls4lb[] = "/1/lfo2pls";
char masterlb[] = "/1/masterVal";
char patchlb[] = "/1/patchVal";
char lpflb[] = "/1/lpfVal";
char hpflb[] = "/1/hpfVal";
char lowlb[] = "/1/filterL";
char highlb[] = "/1/filterH";
char bandlb[] = "/1/filterB";
char notlb[] = "/1/filterN";
char xy1m1[] = "/1/XY2O";
char xy1m2[] = "/1/XY4O";
char xy1m3[] = "/1/XYFR";
char xy1m4[] = "/1/XYLH";


////////////////////////////////////////////////////
//  GUI control address variables (controls that can be initialized)
//  page 1 and 5
char coarse1ctl[] = "/1/coarse1";
char pwm1ctl[] = "/1/pwm1";
char coarse2ctl[] = "/1/coarse2";
char pwm2ctl[] = "/1/pwm2";
char rate1ctl[] = "/1/rate1";
char pwm3ctl[] = "/1/pwm1lfo";
char rate2ctl[] = "/1/rate2";
char pwm4ctl[] = "/1/pwm2lfo";
char adsr1ctl[] = "/1/adsr1";
char adsr2ctl[] = "/1/adsr2";
char mix1ctl[] = "/1/mixer1";
char mix2ctl[] = "/1/mixer2";
char mix3ctl[] = "/1/mixer3";
char mix4ctl[] = "/1/mixer4";
char mix5ctl[] = "/1/mixer5";
char mix6ctl[] = "/1/mixer6";
char masterctl[] = "/1/master";
char mutectl[] = "/1/mute";
char holdctl[] = "/1/hold";
char lpfctl[] = "/1/lpf";
char hpfctl[] = "/1/hpf";

  
////////////////////////////////////////////////////
void setup() {
  //I2C
 Wire.begin();
 Serial.begin(115200);
 delay(100);
  //WIFI  
  //WiFi.config(IPAddress(<Node>), IPAddress(<iPad>), IPAddress(<subnet mask>));                   // TEMPLATE 
   WiFi.config(IPAddress(192,168,0,123),IPAddress(192,168,0,4), IPAddress(255,255,255,0));   //paco CR apt
   // WiFi.config(IPAddress(10,12,10,78), IPAddress(10,26,42,73), IPAddress(255,255,248,0)); //IASTATE

   // Connect to WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" ");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Udp.begin(localPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localPort);
    synthInit();
}
////////////////////////////////////////////////////
void loop() {
  //inputContents();
   GUI_IN();
  delay(1);
}
////////////////////////////////////////////////////
void synthInit(){
  
}
////////////////////////////////////////////////////
void GUI_IN(){
  OSCMessage gui_in;
  int msgSize;
  if((msgSize = Udp.parsePacket())>0){
    while(msgSize --)
      gui_in.fill(Udp.read());
      if(!gui_in.hasError()){
        //gui_in.route("/<page number>/<gui control name>", <function that handles that controls' input>);
        if( page == 1){
        gui_in.route("/1/coarse1", osc1pitchOffset);
        gui_in.route("/1/oct1P", osc1oct);
        gui_in.route("/1/oct1N", osc1oct);
        gui_in.route("/1/pwm1", osc1pwm);
        gui_in.route("/1/wave1", osc1wave);
        gui_in.route("/1/coarse2", osc2pitchOffset);
        gui_in.route("/1/oct2P", osc2oct);
        gui_in.route("/1/oct2N", osc2oct);
        gui_in.route("/1/pwm2", osc2pwm);
        gui_in.route("/1/wave2", osc2wave);
        gui_in.route("/1/rate1", lfo1rate);
        gui_in.route("/1/pwm1lfo", lfo1pwm);
        gui_in.route("/1/wave1lfo", lfo1wave);
        gui_in.route("/1/rate2", lfo2rate);
        gui_in.route("/1/pwm2lfo", lfo2pwm);
        gui_in.route("/1/wave2lfo", lfo2wave);
        gui_in.route("/1/adsr1/1", adsr1);
        gui_in.route("/1/adsr1/2", adsr1);
        gui_in.route("/1/adsr1/3", adsr1);
        gui_in.route("/1/adsr1/4", adsr1);
        gui_in.route("/1/adsr2/1", adsr2);
        gui_in.route("/1/adsr2/2", adsr2);
        gui_in.route("/1/adsr2/3", adsr2);
        gui_in.route("/1/adsr2/4", adsr2);
        gui_in.route("/1/mixer1", mix1);
        gui_in.route("/1/mixer2", mix2);
        gui_in.route("/1/mixer3", mix3);
        gui_in.route("/1/mixer4", mix4);
        gui_in.route("/1/mixer5", mix5);
        gui_in.route("/1/mixer6", mix6);
        gui_in.route("/1/master", outAmp);
        gui_in.route("/1/mute", muteAll);
        gui_in.route("/1/hold", holdNote);
        gui_in.route("/1/save", savePatch);
        gui_in.route("/1/patchP", changePatch);
        gui_in.route("/1/patchN", changePatch);
        gui_in.route("/1/filter", filterSelect);
        gui_in.route("/1/lpf", lpfFreq);
        gui_in.route("/1/hpf", hpfFreq);
        gui_in.route("/1/XYmode", xymode);
        gui_in.route("/1/XY/1", xy1);
        gui_in.route("/1/XY/2", xy2);
        gui_in.route("/1/XY/3", xy1);
        gui_in.route("/1/XY/4", xy2);
        gui_in.route("/1/XY/5", xy1);
       }////////////////////////////////////////////
       /*
       if(page == 2){
          gui_in.route("/2/XYleftMode", xylMode);
          gui_in.route("/2/XYleft/1", xyl1);
          gui_in.route("/2/XYleft/2", xyl2);
          gui_in.route("/2/XYleft/3", xyl1);
          gui_in.route("/2/XYleft/4", xyl2);
          gui_in.route("/2/XYleft/5", xyl1);
          gui_in.route("/2/XYrightMode", xyrMode);
          gui_in.route("/2/XYright/1", xyr1);
          gui_in.route("/2/XYright/2", xyr2);
          gui_in.route("/2/XYright/3", xyr1);
          gui_in.route("/2/XYright/4", xyr2);
          gui_in.route("/2/XYright/5", xyr1);
       }////////////////////////////////////////////
       */
       if(page == 2){
          gui_in.route("/3/modBay/1/1", mb11);
          gui_in.route("/3/modBay/1/2", mb12);
          gui_in.route("/3/modBay/1/3", mb13);
          gui_in.route("/3/modBay/1/4", mb14);
          gui_in.route("/3/modBay/1/5", mb15);
          gui_in.route("/3/modBay/1/6", mb16);
          gui_in.route("/3/modBay/1/7", mb17);
          gui_in.route("/3/modBay/1/8", mb18);
          gui_in.route("/3/modBay/1/9", mb19);
          gui_in.route("/3/modBay/1/10", mb10);
          gui_in.route("/3/modBay/2/1", mb21);
          gui_in.route("/3/modBay/2/2", mb22);
          gui_in.route("/3/modBay/2/3", mb23);
          gui_in.route("/3/modBay/2/4", mb24);
          gui_in.route("/3/modBay/2/5", mb25);
          gui_in.route("/3/modBay/2/6", mb26);
          gui_in.route("/3/modBay/2/7", mb27);
          gui_in.route("/3/modBay/2/8", mb28);
          gui_in.route("/3/modBay/2/9", mb29);
          gui_in.route("/3/modBay/2/10", mb20);
          gui_in.route("/3/modBay/3/1", mb31);
          gui_in.route("/3/modBay/3/2", mb32);
          gui_in.route("/3/modBay/3/3", mb33);
          gui_in.route("/3/modBay/3/4", mb34);
          gui_in.route("/3/modBay/3/5", mb35);
          gui_in.route("/3/modBay/3/6", mb36);
          gui_in.route("/3/modBay/3/7", mb37);
          gui_in.route("/3/modBay/3/8", mb38);
          gui_in.route("/3/modBay/3/9", mb39);
          gui_in.route("/3/modBay/3/10", mb30);
          gui_in.route("/3/modBay/4/1", mb41);
          gui_in.route("/3/modBay/4/2", mb42);
          gui_in.route("/3/modBay/4/3", mb43);
          gui_in.route("/3/modBay/4/4", mb44);
          gui_in.route("/3/modBay/4/5", mb45);
          gui_in.route("/3/modBay/4/6", mb46);
          gui_in.route("/3/modBay/4/7", mb47);
          gui_in.route("/3/modBay/4/8", mb48);
          gui_in.route("/3/modBay/4/9", mb49);
          gui_in.route("/3/modBay/4/10", mb40);
          gui_in.route("/3/modBay/5/1", mb51);
          gui_in.route("/3/modBay/5/2", mb52);
          gui_in.route("/3/modBay/5/3", mb53);
          gui_in.route("/3/modBay/5/4", mb54);
          gui_in.route("/3/modBay/5/5", mb55);
          gui_in.route("/3/modBay/5/6", mb56);
          gui_in.route("/3/modBay/5/7", mb57);
          gui_in.route("/3/modBay/5/8", mb58);
          gui_in.route("/3/modBay/5/9", mb59);
          gui_in.route("/3/modBay/5/10", mb50);
          gui_in.route("/3/modBay/6/1", mb61);
          gui_in.route("/3/modBay/6/2", mb62);
          gui_in.route("/3/modBay/6/3", mb63);
          gui_in.route("/3/modBay/6/4", mb64);
          gui_in.route("/3/modBay/6/5", mb65);
          gui_in.route("/3/modBay/6/6", mb66);
          gui_in.route("/3/modBay/6/7", mb67);
          gui_in.route("/3/modBay/6/8", mb68);
          gui_in.route("/3/modBay/6/9", mb69);
          gui_in.route("/3/modBay/6/10", mb60);
          gui_in.route("/3/modBay/7/1", mb71);
          gui_in.route("/3/modBay/7/2", mb72);
          gui_in.route("/3/modBay/7/3", mb73);
          gui_in.route("/3/modBay/7/4", mb74);
          gui_in.route("/3/modBay/7/5", mb75);
          gui_in.route("/3/modBay/7/6", mb76);
          gui_in.route("/3/modBay/7/7", mb77);
          gui_in.route("/3/modBay/7/8", mb78);
          gui_in.route("/3/modBay/7/9", mb79);
          gui_in.route("/3/modBay/7/10", mb70);
          gui_in.route("/3/modBay/8/1", mb81);
          gui_in.route("/3/modBay/8/2", mb82);
          gui_in.route("/3/modBay/8/3", mb83);
          gui_in.route("/3/modBay/8/4", mb84);
          gui_in.route("/3/modBay/8/5", mb85);
          gui_in.route("/3/modBay/8/6", mb86);
          gui_in.route("/3/modBay/8/7", mb87);
          gui_in.route("/3/modBay/8/8", mb88);
          gui_in.route("/3/modBay/8/9", mb89);
          gui_in.route("/3/modBay/8/10", mb80);
       }////////////////////////////////////////////
       if(page == 3){
          gui_in.route("/4/36", midi36);
          gui_in.route("/4/37", midi37);
          gui_in.route("/4/38", midi38);
          gui_in.route("/4/39", midi39);
          gui_in.route("/4/40", midi40);
          gui_in.route("/4/41", midi41);
          gui_in.route("/4/42", midi42);
          gui_in.route("/4/43", midi43);
          gui_in.route("/4/44", midi44);
          gui_in.route("/4/45", midi45);
          gui_in.route("/4/46", midi46);
          gui_in.route("/4/47", midi47);
          gui_in.route("/4/48", midi48);
          gui_in.route("/4/49", midi49);
          gui_in.route("/4/50", midi50);
          gui_in.route("/4/51", midi51);
          gui_in.route("/4/52", midi52);
          gui_in.route("/4/53", midi53);
          gui_in.route("/4/54", midi54);
          gui_in.route("/4/55", midi55);
          gui_in.route("/4/56", midi56);
          gui_in.route("/4/57", midi57);
          gui_in.route("/4/58", midi58);
          gui_in.route("/4/59", midi59);
          gui_in.route("/4/60", midi60);
          gui_in.route("/4/61", midi61);
          gui_in.route("/4/62", midi62);
          gui_in.route("/4/63", midi63);
          gui_in.route("/4/64", midi64);
          gui_in.route("/4/65", midi65);
          gui_in.route("/4/66", midi66);
          gui_in.route("/4/67", midi67);
          gui_in.route("/4/68", midi68);
          gui_in.route("/4/69", midi69);
          gui_in.route("/4/70", midi70);
          gui_in.route("/4/71", midi71);
          gui_in.route("/4/72", midi72);
          gui_in.route("/4/73", midi73);
          gui_in.route("/4/74", midi74);
          gui_in.route("/4/75", midi75);
          gui_in.route("/4/76", midi76);
          gui_in.route("/4/77", midi77);
          gui_in.route("/4/78", midi78);
          gui_in.route("/4/79", midi79);
          gui_in.route("/4/80", midi80);
          gui_in.route("/4/81", midi81);
          gui_in.route("/4/82", midi82);
          gui_in.route("/4/83", midi83);
          gui_in.route("/4/84", midi84);
          gui_in.route("/4/85", midi85);
          gui_in.route("/4/86", midi86);
          gui_in.route("/4/87", midi87);
          gui_in.route("/4/88", midi88);
       }////////////////////////////////////////////
       if(page == 4){
          gui_in.route("/5/coarse1", osc1pitchOffset);
          gui_in.route("/5/oct1P", osc1oct);
          gui_in.route("/5/oct1N", osc1oct);
          gui_in.route("/5/pwm1", osc1pwm);
          gui_in.route("/5/wave1", osc1wave);
          gui_in.route("/5/coarse2", osc2pitchOffset);
          gui_in.route("/5/oct2P", osc2oct);
          gui_in.route("/5/oct2N", osc2oct);
          gui_in.route("/5/pwm2", osc2pwm);
          gui_in.route("/5/wave2", osc2wave);
          gui_in.route("/5/rate1", lfo1rate);
          gui_in.route("/5/pwm1lfo", lfo1pwm);
          gui_in.route("/5/wave1lfo", lfo1wave);
          gui_in.route("/5/rate2", lfo2rate);
          gui_in.route("/5/pwm2lfo", lfo2pwm);
          gui_in.route("/5/wave2lfo", lfo2wave);
          gui_in.route("/5/adsr1/1", adsr1);
          gui_in.route("/5/adsr1/2", adsr1);
          gui_in.route("/5/adsr1/3", adsr1);
          gui_in.route("/5/adsr1/4", adsr1);
          gui_in.route("/5/adsr2/1", adsr2);
          gui_in.route("/5/adsr2/2", adsr2);
          gui_in.route("/5/adsr2/3", adsr2);
          gui_in.route("/5/adsr2/4", adsr2);
          gui_in.route("/5/mixer1", mix1);
          gui_in.route("/5/mixer2", mix2);
          gui_in.route("/5/mixer3", mix3);
          gui_in.route("/5/mixer4", mix4);
          gui_in.route("/5/mixer5", mix5);
          gui_in.route("/5/mixer6", mix6);
          gui_in.route("/5/master", outAmp);
          gui_in.route("/5/mute", muteAll);
          gui_in.route("/5/hold", holdNote);
          gui_in.route("/5/save", savePatch);
          gui_in.route("/5/patchP", changePatch);
          gui_in.route("/5/patchN", changePatch);
          gui_in.route("/5/filter", filterSelect);
          gui_in.route("/5/lpf", lpfFreq);
          gui_in.route("/5/hpf", hpfFreq);
          gui_in.route("/5/36", midi36);
          gui_in.route("/5/37", midi37);
          gui_in.route("/5/38", midi38);
          gui_in.route("/5/39", midi39);
          gui_in.route("/5/40", midi40);
          gui_in.route("/5/41", midi41);
          gui_in.route("/5/42", midi42);
          gui_in.route("/5/43", midi43);
          gui_in.route("/5/44", midi44);
          gui_in.route("/5/45", midi45);
          gui_in.route("/5/46", midi46);
          gui_in.route("/5/47", midi47);
          gui_in.route("/5/48", midi48);
          gui_in.route("/5/49", midi49);
          gui_in.route("/5/50", midi50);
          gui_in.route("/5/51", midi51);
          gui_in.route("/5/52", midi52);
          gui_in.route("/5/53", midi53);
          gui_in.route("/5/54", midi54);
          gui_in.route("/5/55", midi55);
          gui_in.route("/5/56", midi56);
          gui_in.route("/5/57", midi57);
          gui_in.route("/5/58", midi58);
          gui_in.route("/5/59", midi59);
          gui_in.route("/5/60", midi60);
       }
     }
  }
}
////////////////////////////////////////////////////
void send_to_GUI(char destOSCAdd[], float dataSend){
  //Serial.println("debug:entered sendNodeOSC");
  // /*
   Serial.println(destOSCAdd);
  //Serial.println(charSend);
  Serial.println(dataSend);
  Serial.println(destIpad); // */
  OSCMessage send_GUI(destOSCAdd);
  send_GUI.add(dataSend);
  Udp.beginPacket(destIpad,destPort);
  send_GUI.send(Udp);
  Udp.endPacket();
  send_GUI.empty();
}
////////////////////////////////////////////////////
void send_to_GUI_adsr(char destOSCAdd[], int adsrFader, int dataSend){
  //Serial.println("debug:entered sendNodeOSC");
 /* Serial.println(destOSCAdd);
  //Serial.println(charSend);
  Serial.println(dataSend);
  Serial.println(destIpad); // */
  OSCMessage send_GUI(destOSCAdd);
  send_GUI.add(dataSend);
  Udp.beginPacket(destIpad,destPort);
  send_GUI.send(Udp);
  Udp.endPacket();
  send_GUI.empty();
}
////////////////////////////////////////////////////
void send_to_4LEDselect(int LEDset, int LEDpick){
  //sin1lb, tri1lb, saw1lb, pls1lb
  Serial.printf("%d %d", LEDset, LEDpick);
  char sinlb[9];
     char trilb[9];
     char sawlb[9];
     char plslb[9];
  if(LEDset == 1){
    if(LEDpick == 0){
    send_to_GUI(sin1lb, 1);
    send_to_GUI(tri1lb, 0);
    send_to_GUI(saw1lb, 0);
    send_to_GUI(pls1lb, 0);
  }
  else if(LEDpick == 1){
    send_to_GUI(sin1lb, 0);
    send_to_GUI(tri1lb, 1);
    send_to_GUI(saw1lb, 0);
    send_to_GUI(pls1lb, 0);
  }
  else if(LEDpick == 2){
    send_to_GUI(sin1lb, 0);
    send_to_GUI(tri1lb, 0);
    send_to_GUI(saw1lb, 1);
    send_to_GUI(pls1lb, 0);
  }
  else if(LEDpick == 3){
    send_to_GUI(sin1lb, 0);
    send_to_GUI(tri1lb, 0);
    send_to_GUI(saw1lb, 0);
    send_to_GUI(pls1lb, 1);
  }
  }
  else if(LEDset == 2){
    if(LEDpick == 0){
    send_to_GUI(sin2lb, 1);
    send_to_GUI(tri2lb, 0);
    send_to_GUI(saw2lb, 0);
    send_to_GUI(pls2lb, 0);
  }
  else if(LEDpick == 1){
    send_to_GUI(sin2lb, 0);
    send_to_GUI(tri2lb, 1);
    send_to_GUI(saw2lb, 0);
    send_to_GUI(pls2lb, 0);
  }
  else if(LEDpick == 2){
    send_to_GUI(sin2lb, 0);
    send_to_GUI(tri2lb, 0);
    send_to_GUI(saw2lb, 1);
    send_to_GUI(pls2lb, 0);
  }
  else if(LEDpick == 3){
    send_to_GUI(sin2lb, 0);
    send_to_GUI(tri2lb, 0);
    send_to_GUI(saw2lb, 0);
    send_to_GUI(pls2lb, 1);
  }
  }
  else if(LEDset == 3){
     if(LEDpick == 0){
    send_to_GUI(sin3lb, 1);
    send_to_GUI(tri3lb, 0);
    send_to_GUI(saw3lb, 0);
    send_to_GUI(pls3lb, 0);
  }
  else if(LEDpick == 1){
    send_to_GUI(sin3lb, 0);
    send_to_GUI(tri3lb, 1);
    send_to_GUI(saw3lb, 0);
    send_to_GUI(pls3lb, 0);
  }
  else if(LEDpick == 2){
    send_to_GUI(sin3lb, 0);
    send_to_GUI(tri3lb, 0);
    send_to_GUI(saw3lb, 1);
    send_to_GUI(pls3lb, 0);
  }
  else if(LEDpick == 3){
    send_to_GUI(sin3lb, 0);
    send_to_GUI(tri3lb, 0);
    send_to_GUI(saw3lb, 0);
    send_to_GUI(pls3lb, 1);
  }
  }
  else if(LEDset == 4){
    if(LEDpick == 0){
    send_to_GUI(sin4lb, 1);
    send_to_GUI(tri4lb, 0);
    send_to_GUI(saw4lb, 0);
    send_to_GUI(pls4lb, 0);
  }
  else if(LEDpick == 1){
    send_to_GUI(sin4lb, 0);
    send_to_GUI(tri4lb, 1);
    send_to_GUI(saw4lb, 0);
    send_to_GUI(pls4lb, 0);
  }
  else if(LEDpick == 2){
    send_to_GUI(sin4lb, 0);
    send_to_GUI(tri4lb, 0);
    send_to_GUI(saw4lb, 1);
    send_to_GUI(pls4lb, 0);
  }
  else if(LEDpick == 3){
    send_to_GUI(sin4lb, 0);
    send_to_GUI(tri4lb, 0);
    send_to_GUI(saw4lb, 0);
    send_to_GUI(pls4lb, 1);
  }
  }
  else if(LEDset == 5){
    if(LEDpick == 0){
    send_to_GUI(lowlb, 1);
    send_to_GUI(highlb, 0);
    send_to_GUI(bandlb, 0);
    send_to_GUI(notlb, 0);
  }
  else if(LEDpick == 1){
    send_to_GUI(lowlb, 0);
    send_to_GUI(highlb, 1);
    send_to_GUI(bandlb, 0);
    send_to_GUI(notlb, 0);
  }
  else if(LEDpick == 2){
    send_to_GUI(lowlb, 0);
    send_to_GUI(highlb, 0);
    send_to_GUI(bandlb, 1);
    send_to_GUI(notlb, 0);
  }
  else if(LEDpick == 3){
   send_to_GUI(lowlb, 0);
    send_to_GUI(highlb, 0);
    send_to_GUI(bandlb, 0);
    send_to_GUI(notlb, 1);
  }
  }
  else if(LEDset == 6){
    if(LEDpick == 0){
    send_to_GUI(xy1m1, 1);
    send_to_GUI(xy1m2, 0);
    send_to_GUI(xy1m3, 0);
    send_to_GUI(xy1m4, 0);
  }
  else if(LEDpick == 1){
    send_to_GUI(xy1m1, 0);
    send_to_GUI(xy1m2, 1);
    send_to_GUI(xy1m3, 0);
    send_to_GUI(xy1m4, 0);
  }
  else if(LEDpick == 2){
    send_to_GUI(xy1m1, 0);
    send_to_GUI(xy1m2, 0);
    send_to_GUI(xy1m3, 1);
    send_to_GUI(xy1m4, 0);
  }
  else if(LEDpick == 3){
   send_to_GUI(xy1m1, 0);
    send_to_GUI(xy1m2, 0);
    send_to_GUI(xy1m3, 0);
    send_to_GUI(xy1m4, 1);
  }
  }
}

////////////////////////////////////////////////////
void save_all_patch_vals(int savePatch){
  //fill patch array with all values from current patch
  //save patch array to memory slot for that patch
}
////////////////////////////////////////////////////
void set_all_patch_vals(int loadPatch){
  //fill patch array with loaded values from memory
  //set all parameters with array values in circuit
  //set all parameters with array values in GUI
}

////////////////////////////////////////////////////
void inputContents(){
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
}
////////////////////////////////////////////////////
// TEMPLATE: function for GUI input 

//void <function that handles that controls' input>(OSCMessage &msg, int addrOffset){
//  <global float variable> = msg.getFloat(0);
//  float <private float variable> = msg.getFloat(0);
//  byte <send via i2c variable> = floor(<float var>*<scale int>);
//  int <send to GUI variable> = floor(<float var>*<scale int>);
//  send_to_GUI("/<page#>/<gui control name>", <send to GUI variable>);
//  send_to_GUI_adsr("/<page#>/<gui control name>", <adsrFader>, <send to GUI variable>);
//  send_to_GUI_modBay("/3/modBay", <X 1-12>, <Y 1-8>, <0/1>);
//  //do stuff with new data
// }
 
 ////////////////////////////////////////////////////
 void sendPitchToOsc(){
  
 }
 
  ////////////////////////////////////////////////////
  //        GUI FUNCTIONS                           //
  ////////////////////////////////////////////////////
  
 //         OSCILLATORS
 /////////////////////////////////////////////////////
 void osc1pitchOffset(OSCMessage &msg, int addrOffset){
  float coarse1new = msg.getFloat(0);
  if (coarse1 != coarse1new) {
    coarse1 = coarse1new;
  }
  else {
    coarse1val = coarse1*24.0;
    String cval1 = String(coarse1val, 1);
    coarse1val = cval1.toFloat();
    send_to_GUI(coarse1lb, coarse1val);
    //send to pwm1 digi pot here
  }
  /*
  if( (coarse1new == coarse1) || (abs((24*coarse1) - (24*coarse1new))>1)){
    coarse1val = ceil(coarse1new*25);
    
  }
  else{
    coarse1 = coarse1new;
  }
  */
  ///NOT DONE!!!!!!!!!!!!!!
 }


 ////////////////////////////////////////////////////
 void osc1oct(OSCMessage &msg, int addrOffset){
  float oct1new = msg.getFloat(0);
  oct1val = oct1val + oct1new;
  if( oct1val == -1 ){
    oct1val = 0;
  }
  else if( oct1val == 9 ){
    oct1val = 8;
  }
  send_to_GUI(oct1lb, int (oct1val));
  //send to osc1 dac's for octave offset
 }
 
 ////////////////////////////////////////////////////
 void osc1pwm(OSCMessage &msg, int addrOffset){
  float pwm1new = msg.getFloat(0);
  if (pwm1 != pwm1new) {
    pwm1 = pwm1new;
  }
  else {
    pwm1val = pwm1*100;
    send_to_GUI(pwm1lb, int(pwm1val));
    //send to pwm1 digi pot here
  }
 }
 
 ////////////////////////////////////////////////////
 void osc1wave(OSCMessage &msg, int addrOffset){
  wave1 = msg.getFloat(0);
  wave1val = wave1val + wave1;
  if(wave1val == 4){
    wave1val = 0;
  }
  send_to_4LEDselect(1,wave1val);
  //send to mux wave select here
 }
 
 ////////////////////////////////////////////////////
 void osc2pitchOffset(OSCMessage &msg, int addrOffset){
  float coarse2new = msg.getFloat(0);
  if (coarse2 != coarse2new) {
    coarse2 = coarse2new;
  }
  else {
    coarse2val = coarse2*24.0;
    String cval2 = String(coarse2val, 1);
    coarse2val = cval2.toFloat();
    send_to_GUI(coarse2lb, coarse2val);
    //send to pwm1 digi pot here
  }
 }

 
 ////////////////////////////////////////////////////
 void osc2oct(OSCMessage &msg, int addrOffset){
  float oct2new = msg.getFloat(0);
  oct2val = oct2val + oct2new;
  if( oct2val == -1 ){
    oct2val = 0;
  }
  else if( oct2val == 9 ){
    oct2val = 8;
  }
  send_to_GUI(oct2lb, int (oct2val));
  //send to osc1 dac's for octave offset
 }

 
 ////////////////////////////////////////////////////
 void osc2pwm(OSCMessage &msg, int addrOffset){
  float pwm2new = msg.getFloat(0);
  if (pwm2 != pwm2new) {
    pwm2 = pwm2new;
  }
  else {
    pwm2val = pwm2*100;
    send_to_GUI(pwm2lb, int(pwm2val));
    //send to pwm1 digi pot here
  }
 }

 
 ////////////////////////////////////////////////////
 void osc2wave(OSCMessage &msg, int addrOffset){
  wave2 = msg.getFloat(0);
  wave2val = wave2val + wave2;
  if(wave2val == 4){
    wave2val = 0;
  }
  send_to_4LEDselect(2,wave2val);
  //send to mux wave select here
 }

  //          LFO
 ////////////////////////////////////////////////////
 void lfo1rate(OSCMessage &msg, int addrOffset){
  float rate1new = msg.getFloat(0);
  if (rate1 != rate1new) {
    rate1 = rate1new;
  }
  else {
    rate1val = floor(rate1*120);
    send_to_GUI(rate1lb, rate1val);
    //send to pwm1 digi pot here
  }
 }
 
 ////////////////////////////////////////////////////
 void lfo1pwm(OSCMessage &msg, int addrOffset){
  float pwm1lfonew = msg.getFloat(0);
  if (pwm1lfo != pwm1lfonew) {
    pwm1lfo = pwm1lfonew;
  }
  else {
    pwm1lfoVal = pwm1lfo*100;
    send_to_GUI(pwm1lfolb, int(pwm1lfoVal));
    //send to pwm1 digi pot here
  }
 }
 
 ////////////////////////////////////////////////////
 void lfo1wave(OSCMessage &msg, int addrOffset){
  wave3 = msg.getFloat(0);
  wave3val = wave3val + wave3;
  if(wave3val == 4){
    wave3val = 0;
  }
  send_to_4LEDselect(3,wave3val);
  //send to mux wave select here
 }
 
 ////////////////////////////////////////////////////
 void lfo2rate(OSCMessage &msg, int addrOffset){
  float rate2new = msg.getFloat(0);
  if (rate2 != rate2new) {
    rate2 = rate2new;
  }
  else {
    rate2val = floor(rate2*120);
    send_to_GUI(rate2lb, rate2val);
    //send to rate1 dac's here
  }
 }

 ////////////////////////////////////////////////////
 void lfo2pwm(OSCMessage &msg, int addrOffset){
  float pwm2lfonew = msg.getFloat(0);
  if (pwm2lfo != pwm2lfonew) {
    pwm2lfo = pwm2lfonew;
  }
  else {
    pwm2lfoVal = pwm2lfo*100;
    send_to_GUI(pwm2lfolb, int(pwm2lfoVal));
    //send to pwm1 digi pot here
  }
 }
 
 ////////////////////////////////////////////////////
 void lfo2wave(OSCMessage &msg, int addrOffset){
  wave4 = msg.getFloat(0);
  wave4val = wave4val + wave4;
  if(wave4val == 4){
    wave4val = 0;
  }
  send_to_4LEDselect(4,wave4val);
  //send to mux wave select here
 }

 //         DIGITAL ADSR
 ////////////////////////////////////////////////////
 void adsr1(OSCMessage &msg, int addrOffset){
  
 }

 
 ////////////////////////////////////////////////////
 void adsr2(OSCMessage &msg, int addrOffset){
  
 }

 //         MIXER
 ////////////////////////////////////////////////////
 void mix1(OSCMessage &msg, int addrOffset){
  float mix1new = msg.getFloat(0);
  if(mix1new != mixer1){
    mixer1 = mix1new;
  }
  else{
   int mix1 = (int) floor(mixer1*100);
   //send to mixer algor
  }
 }
 
 ////////////////////////////////////////////////////
 void mix2(OSCMessage &msg, int addrOffset){
  float mix2new = msg.getFloat(0);
  if(mix2new != mixer2){
    mixer2 = mix2new;
  }
  else{
   int mix2 = (int) floor(mixer2*100);
   //send to mixer algor
  }
 }
 
 ////////////////////////////////////////////////////
 void mix3(OSCMessage &msg, int addrOffset){
  float mix3new = msg.getFloat(0);
  if(mix3new != mixer3){
    mixer3 = mix3new;
  }
  else{
   int mix3 = (int) floor(mixer3*100);
   //send to mixer algor
  }
 }
 
 ////////////////////////////////////////////////////
 void mix4(OSCMessage &msg, int addrOffset){
  float mix4new = msg.getFloat(0);
  if(mix4new != mixer4){
    mixer4 = mix4new;
  }
  else{
   int mix4 = (int) floor(mixer4*100);
   //send to mixer algor
  }
 }
 
 ////////////////////////////////////////////////////
 void mix5(OSCMessage &msg, int addrOffset){
  float mix5new = msg.getFloat(0);
  if(mix5new != mixer5){
    mixer5 = mix5new;
  }
  else{
   int mix5 = (int) floor(mixer5*100);
   //send to mixer algor
  }
 }
 
 ////////////////////////////////////////////////////
 void mix6(OSCMessage &msg, int addrOffset){
  float mix6new = msg.getFloat(0);
  if(mix6new != mixer6){
    mixer6 = mix6new;
  }
  else{
   int mix6 = (int) floor(mixer6*100);
   //send to mixer algor
  }
 }

 //         OUTPUT AMP
 ////////////////////////////////////////////////////
 void outAmp(OSCMessage &msg, int addrOffset){
  float masterNew = msg.getFloat(0);
  if(masterNew != master){
    master = masterNew;
  }
  else {
  masterVal = (int) floor(master*100);
  send_to_GUI(masterlb, masterVal);
  masterVal = (int) floor(master*255);
  //send_to_outAmp(masterVal);
  }
 }

 
 ////////////////////////////////////////////////////
 void muteAll(OSCMessage &msg, int addrOffset){
  mute = (boolean) msg.getFloat(0);
 }
 
 ////////////////////////////////////////////////////
 void holdNote(OSCMessage &msg, int addrOffset){
  hold = (boolean) msg.getFloat(0);
 }

  //          SYSTEM 
 ////////////////////////////////////////////////////
 void savePatch(OSCMessage &msg, int addrOffset){
  save = msg.getFloat(0);
  if (save == 0){
    save_all_patch_vals(patchVal); //fill array determined by user and save to memory
    //save_all_patch_vals(<patchVal determined by U/D while save ==1>);
  }
 }

 ////////////////////////////////////////////////////
 void changePatch(OSCMessage &msg, int addrOffset){
  float patchUD = msg.getFloat(0);
  patchVal = patchVal + patchUD;
  if(patchVal == 9){
    patchVal = 1;
  }
  else if (patchVal == 0){
    patchVal = 8;
  }
  send_to_GUI(patchlb, patchVal);
  if(save == 0){
    patch = patchVal;
   set_all_patch_vals(patch);//reads array from memory and set all patch values
  }
  else{
    //nothing? if save == 1, we don't load & set the patch values since we're picking
    //a patch to overwrite. We dont't want to overwrite our current patch with a saved patch.
  }
 }

 //         FILTERS
 ////////////////////////////////////////////////////
 void filterSelect(OSCMessage &msg, int addrOffset){
  filter = msg.getFloat(0);
  filterType = filterType + filter;
  if(filterType == 4){
    filterType = 0;
  }
  send_to_4LEDselect(5,filterType);
  //send to muxs filter select here
 }
 
 ////////////////////////////////////////////////////
 void lpfFreq(OSCMessage &msg, int addrOffset){
   float lpfnew = msg.getFloat(0);
  if (lpfnew != lpf) {
    lpf = lpfnew;
  }
  else {
    lpfVal = lpf*100;
    send_to_GUI(lpflb, int(lpfVal));
    //send to lpf digi pots here
  }
 }
 
 ////////////////////////////////////////////////////
 void hpfFreq(OSCMessage &msg, int addrOffset){
    float hpfnew = msg.getFloat(0);
  if (hpfnew != hpf) {
    hpf = hpfnew;
  }
  else {
    hpfVal = hpf*100;
    send_to_GUI(hpflb, int(hpfVal));
    //send to hpf digi pots here
  }
 }

 //         TOUCHPAD 1         
 ////////////////////////////////////////////////////
 void xymode(OSCMessage &msg, int addrOffset){
  XYmode = msg.getFloat(0);
  XYMp1 = XYMp1 + XYmode;
  if(XYMp1 == 4){
    XYMp1 = 0;
  }
  send_to_4LEDselect(6,XYMp1);
  //send to muxs filter select here
 }

 ////////////////////////////////////////////////////
 void xy1(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void xy2(OSCMessage &msg, int addrOffset){
  
 }
 //         TOUCHPAD LEFT        
 ////////////////////////////////////////////////////
 void xylMode(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void xyl1(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void xyl2(OSCMessage &msg, int addrOffset){
  
 }

  //         TOUCHPAD RIGHT        
 ////////////////////////////////////////////////////
 void xyrMode(OSCMessage &msg, int addrOffset){
  
 }
 
 ////////////////////////////////////////////////////
 void xyr1(OSCMessage &msg, int addrOffset){
  
 }
 
 ////////////////////////////////////////////////////
 void xyr2(OSCMessage &msg, int addrOffset){
  
 }
 
 //         KEYS 36-88
 ////////////////////////////////////////////////////
 void midi36(OSCMessage &msg, int addrOffset){
  
 }
 
 ////////////////////////////////////////////////////
 void midi37(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi38(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi39(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi40(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi41(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi42(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi43(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi44(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi45(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi46(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi47(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi48(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi49(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi50(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi51(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi52(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi53(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi54(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi55(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi56(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi57(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi58(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi59(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi60(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi61(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi62(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi63(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi64(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi65(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi66(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi67(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi68(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi69(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi70(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi71(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi72(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi73(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi74(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi75(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi76(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi77(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi78(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi79(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi80(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi81(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi82(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi83(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi84(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi85(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi86(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void midi87(OSCMessage &msg, int addrOffset){
  
 } 
 
 ////////////////////////////////////////////////////
 void midi88(OSCMessage &msg, int addrOffset){
  
 }

 //         MODBAY  
 ////////////////////////////////////////////////////
 void mb11(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb12(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb13(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb14(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb15(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb16(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb17(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb18(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb19(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb10(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb21(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb22(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb23(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb24(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb25(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb26(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb27(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb28(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb29(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb20(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb31(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb32(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb33(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb34(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb35(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb36(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb37(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb38(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb39(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb30(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb41(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb42(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb43(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb44(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb45(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb46(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb47(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb48(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb49(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb40(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb51(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb52(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb53(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb54(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb55(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb56(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb57(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb58(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb59(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb50(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb61(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb62(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb63(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb64(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb65(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb66(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb67(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb68(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb69(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb60(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb71(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb72(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb73(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb74(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb75(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb76(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb77(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb78(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb79(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb70(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb81(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb82(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb83(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb84(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb85(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb86(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb87(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb88(OSCMessage &msg, int addrOffset){
  
 }

 ////////////////////////////////////////////////////
 void mb89(OSCMessage &msg, int addrOffset){
  
 }

  ////////////////////////////////////////////////////
 void mb80(OSCMessage &msg, int addrOffset){
  
 }
