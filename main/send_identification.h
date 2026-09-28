// Author: Joseph Cowell | Student ID: 104888857
// Send Identification function

String SDI_v = "14";
String COURSE_CODE = "ENG20009";
String STUDENT_ID = "104888857";  // 9 digits: mmmmmm (first 6) + vvv (last 3)
//also just gonna assume it should be my ID? lol

void send_identification(int address) {
  String id = String(address);
  id += SDI_v;
  id += COURSE_CODE;
  id += STUDENT_ID;
  id += "\r\n";
  Serial.print(id);
}
