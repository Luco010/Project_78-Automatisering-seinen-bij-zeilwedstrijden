<template>
  <div id="app">
    <div class="content">
      <div>
        <GuiScreen/>
        <HornVisualizer ref="horn"/>
      </div>  
      <div class="button-container">
        <NumberSelector/>
        <PushButton text="Race Afbreken" @click="handleButtonClick(1)"/>
        <PushButton text="Uitstel" @click="handleButtonClick(2)"/>
        <PushButton text="Toeter" @mousedown="holdHonk()" @click="handleButtonClick(3)"/>
        <PushButton text="Finish" @click="handleButtonClick(4)"/>
        <PushButton text="Start / Stop" @click="handleButtonClick(5)"/>
      </div>
    </div>
  </div>
</template>

<script>
import GuiScreen from './components/ScreenSaver'
import PushButton from './components/PushButton'
import NumberSelector from './components/NumberSelector'
import HornVisualizer from './components/HornVisualizer.vue'

export default {
  name: 'App',

  components: {
    GuiScreen,
    PushButton,
    NumberSelector,
    HornVisualizer
  },
  methods: {
    handleButtonClick(buttonNumber) {
      const horn = this.$refs.horn;
      switch (buttonNumber) {
        case 1:
          alert('De race word afgebroken, er gaan drie toeters achterelkaar af en nog een toeter één minuut vooor het waarschuwingssignaal.');
          horn.requestHonk(3);
          horn.queuHonk(5000);
          break;
        case 2:
          alert('De race word uitgesteld, er gaan twee toeters achterelkaar af en nog een toeter één minuut vooor het waarschuwingssignaal.');
          horn.requestHonk(2);
          horn.queuHonk(5000);
          break;
        case 3:
          horn.honk(0);
          break;
        case 4:
          alert('De race is afgelopen.');
          break;
        case 5:
          alert('Start / Stop');
          break;
        default:
          break;
      }
    },

    holdHonk(){
      const horn = this.$refs.horn;
      horn.honk(1);
    }
  }
}
</script>

<style>
#app {
  display: flex;
  justify-content: center;
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}

.content {
  display: grid;
  grid-template-columns: 1fr 1fr;
}

.button-container {
  display: flex;
  flex-direction: column;
  justify-content: flex-start;
  margin-left: 300px;
}

.button-container > * {
  margin-bottom: 10px;
}
</style>
