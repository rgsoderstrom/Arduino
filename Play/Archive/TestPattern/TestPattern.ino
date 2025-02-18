

void setup() 
{
  pinMode (2, OUTPUT);
  pinMode (3, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (6, OUTPUT);
  pinMode (7, OUTPUT);
  pinMode (8, OUTPUT);
  pinMode (9, OUTPUT);
}

unsigned int pattern = 1;

void loop() 
{
  pattern <<= 1;

  if (pattern == 0)
    pattern = 1;
  
  digitalWrite (2, (pattern & 1) != 0 ? 1 : 0);
  digitalWrite (3, (pattern & 2) != 0 ? 1 : 0);
  digitalWrite (4, (pattern & 4) != 0 ? 1 : 0);
  digitalWrite (5, (pattern & 8) != 0 ? 1 : 0);
  digitalWrite (6, (pattern & 16) != 0 ? 1 : 0);
  digitalWrite (7, (pattern & 32) != 0 ? 1 : 0);
  digitalWrite (8, (pattern & 64) != 0 ? 1 : 0);
  digitalWrite (9, (pattern & 128) != 0 ? 1 : 0);
}
