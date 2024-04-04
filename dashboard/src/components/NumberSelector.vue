<template>
    <div>
      <div>
        <label>Select number 1:</label>
        <div>
          <button @click="selectNumber(1, 1)" :class="{ 'selected': selectedNumbers[0] === '1' }">1</button>
          <button @click="selectNumber(1, 2)" :class="{ 'selected': selectedNumbers[0] === '2' }">2</button>
          <button @click="selectNumber(1, 3)" :class="{ 'selected': selectedNumbers[0] === '3' }">3</button>
        </div>
      </div>
      <div>
        <label>Select number 2:</label>
        <div>
          <button @click="selectNumber(2, 1)" :class="{ 'selected': selectedNumbers[1] === '1' }">1</button>
          <button @click="selectNumber(2, 2)" :class="{ 'selected': selectedNumbers[1] === '2' }">2</button>
          <button @click="selectNumber(2, 3)" :class="{ 'selected': selectedNumbers[1] === '3' }">3</button>
        </div>
      </div>
      <div>
        <label>Select number 3:</label>
        <div>
          <button @click="selectNumber(3, 1)" :class="{ 'selected': selectedNumbers[2] === '1' }">1</button>
          <button @click="selectNumber(3, 2)" :class="{ 'selected': selectedNumbers[2] === '2' }">2</button>
          <button @click="selectNumber(3, 3)" :class="{ 'selected': selectedNumbers[2] === '3' }">3</button>
        </div>
      </div>
    </div>
  </template>
  
  <script>
  export default {
    data() {
      return {
        selectedNumbers: ['1', '1', '1'] // Default selected numbers
      };
    },
    methods: {
      selectNumber(index, number) {
        this.selectedNumbers[index - 1] = number.toString();
      }
    }
  };
  </script>
  
  <style scoped>
  button {
    border: 1px solid #ccc;
    padding: 5px 10px;
    margin-right: 5px;
    cursor: pointer;
  }
  
  button.selected {
    background-color: #007bff;
    color: white;
  }
  </style>
  