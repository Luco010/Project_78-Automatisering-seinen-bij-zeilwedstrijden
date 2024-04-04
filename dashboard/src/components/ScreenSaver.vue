<template>
    <div class="quarter-top-left">
      <p>{{ text }}</p>
    </div>
  </template>
  
  <script>
  export default {
    props: {
      text: {
        type: String,
        required: true
      }
    }
  };
  </script>
  
  <style scoped>
  .quarter-top-left {
    position: fixed;
    top: 0;
    left: 0;
    width: 50%; /* Adjust as needed */
    height: 50%; /* Adjust as needed */
    padding: 20px;
    background-color: rgba(0, 0, 0, 0.5); /* Semi-transparent black background */
    color: white;
    box-sizing: border-box;
  }
  </style>
  