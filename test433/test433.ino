char *ptrmsg=NULL;
char msg;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
ptrmsg=&msg;
itoa(999,ptrmsg,10);
Serial.print("-->");
Serial.print(*ptrmsg);
Serial.println("<--");

}

void loop() {
  // put your main code here, to run repeatedly: 
  

}
