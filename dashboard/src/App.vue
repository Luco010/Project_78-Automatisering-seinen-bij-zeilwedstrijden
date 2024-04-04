<template>
  <div id="app">
    <div class="content">
      <GuiScreen/>
      <div class="button-container">
        <NumberSelector/>
        <PushButton text="Button 1" @click="handleButtonClick(1)"/>
        <PushButton text="Button 2" @click="handleButtonClick(2)"/>
        <PushButton text="Button 3" @click="handleButtonClick(3)"/>
        <PushButton text="Button 4" @click="handleButtonClick(4)"/>
        <PushButton text="Button 5" @click="handleButtonClick(5)"/>
        <PushButton text="Button 6" @click="handleButtonClick(6)"/>
      </div>
    </div>
  </div>
</template>

<script>
import GuiScreen from './components/ScreenSaver'
import PushButton from './components/PushButton'
import NumberSelector from './components/NumberSelector'

export default {
  name: 'App',
  components: {
    GuiScreen,
    PushButton,
    NumberSelector
  },
  methods: {
    handleButtonClick(buttonNumber) {
      alert('Button ' + buttonNumber + ' clicked!');
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
  display: flex;
  align-items: flex-start;
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
