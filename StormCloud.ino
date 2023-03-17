int lightningPin[] = {12,11,10,9,8,7,6,5,4,3,2};
float countersForLights[] = {0,0,0,0,0,0,0,0,0,0,0};
float timeSetsForLights[] = {0.1f,0.3f,0.2f,0.3f,0.2f,0.1f,0.3f,0.2f,0.3f,0.2f,0.2f};
bool lightsRunning[] = {false, false, false, false, false, false, false, false, false, false, false};
int numberOfLights = 11;

float oldTime = 0.0f;
float currentTime = 0.0f;
float deltaTime = 0.0f;

int mode = 0;
// 0 soft Light
// 1 soft lightning
// 2 heavy lightning
// 3 strong light
float modeTimer = 0.0f;
float maxModeTime = 300.0f;

long randOdds = 100;
long maxTimer = 500000;
long minTimer = 1000;
int deltaTimeScale = 1;
bool runLightningCodes = false;

void setup() 
{
  for(int itr = 0; itr < numberOfLights; itr++)
  {
    pinMode(lightningPin[itr], OUTPUT);
  }
  //mode = 2;
  RunModeState(mode);
}

void UpdateDeltaTime()
{
  oldTime = currentTime;
  currentTime = millis();
  deltaTime = (currentTime - oldTime) / deltaTimeScale;
}

void CheckForNewLightning()
{
  int rand = random(0, randOdds);
  if(rand <= 30)
  {
    rand = random(0, numberOfLights - 1);
    if(lightsRunning[rand] == false)
    {
      int pin = rand;
      rand = random(minTimer, maxTimer);
      float timer = (float)(rand) / 100.0f;
      lightsRunning[pin] = true;
      timeSetsForLights[pin] = timer;
      digitalWrite(lightningPin[pin], HIGH);
    }
  }
}

void RunLightning()
{
  for(int itr = 0; itr < numberOfLights; itr++)
  {
    if(lightsRunning[itr] == true)
    {
      if(timeSetsForLights[itr] > countersForLights[itr])
      {
        digitalWrite(lightningPin[itr], LOW);
        timeSetsForLights[itr] = 0;
        lightsRunning[itr] = false;
      }
      else
      {
        timeSetsForLights[itr] += deltaTime;
      }
    }
  }
}

void loop() {
  UpdateDeltaTime();
  ModeUpdate();
  if( runLightningCodes != false)
  {
    CheckForNewLightning();
    RunLightning();
  }
}

void TurnOffLEDS()
{
  for(int itr = 0; itr < numberOfLights; itr++)
  {
    digitalWrite(lightningPin[itr], LOW);
  }
}

void RunModeState(int state)
{
  switch(state)
    {
      case 0:
        for(int itr = 0; itr < numberOfLights; itr++)
        {
          if(itr % 3 == 0)
          {
            digitalWrite(lightningPin[itr], HIGH);
          }
        }
        runLightningCodes = false;
      break;
      case 1:
        randOdds = 20000;
        maxTimer = 32900;
        minTimer = 32400;
        deltaTimeScale = 1;
        runLightningCodes = true;
      break;
      case 2:
        randOdds = 1000;
        maxTimer = 32820;
        minTimer = 32400;
        deltaTimeScale = 1;
        runLightningCodes = true;
      break;
      case 3:
        for(int itr = 0; itr < numberOfLights; itr++)
        {
          digitalWrite(lightningPin[itr], HIGH);
        }
        runLightningCodes = false;
      break;
    }
}

void ModeUpdate()
{
  if(modeTimer > maxModeTime)
  {
    mode++;
    if(mode > 3)
    {
      mode = 0;
    }
    modeTimer = 0.0f;
    TurnOffLEDS();
    RunModeState(mode);
  }
  else
  {
    modeTimer +=  (deltaTime / 1000);
  }
}